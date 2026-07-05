#define _GNU_SOURCE
#include <signal.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <sys/select.h>
#include "core/bnx_connection.h"

#define BACKLOG 16
#define SEND_BUF_LEN 1024


volatile sig_atomic_t keep_running = 1;
void stop_handler(int _n)
{
    keep_running = 0;
}

int read_file_contents(char *buf, size_t buf_size, char *path)
{
    if (buf == NULL || path == NULL) {
        fprintf(stderr, "invalid argument");
        return -1;
    }

    FILE *fp = fopen(path, "r");
    if (fp == NULL) {
        fprintf(stderr, "fpopen failed(%d)\n", errno);
        return -1;
    }

    size_t byte_read = fread(buf, sizeof(char), buf_size, fp);
    fclose(fp);

    return byte_read;
}

int main(int argc, char **argv)
{
    if (argc < 3) {
        fprintf(stderr, "usage:\n\t%s localhost 8080\n\t%s ::1 8080\n", argv[0], argv[0]);
        fprintf(stderr, "or:\n\t%s localhost 8080 'index.html'\n\t%s ::1 8080 'index.html'\n", argv[0], argv[0]);
        return 1;
    }

    // sigaction
    struct sigaction act = {0};
    act.sa_flags = 0;
    act.sa_handler = stop_handler;

    if (sigaction(SIGINT, &act, NULL) == -1) {
        fprintf(stderr, "sigaction failed\n");
        fprintf(stderr, "stopping program...\n");
        return 1;
    }

    // struct addrinfo / getaddrinfo
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    struct addrinfo *result;
    hints.ai_family = AF_INET6;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_protocol = 0;
    if (getaddrinfo(argv[1], argv[2], &hints, &result) != 0) {
        fprintf(stderr, "getaddrinfo failed(%d)\n", errno);
        fprintf(stderr, "stopping program...\n");
        return 1;
    }

    // socket
    int sfd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (sfd < 0) {
        fprintf(stderr, "socket() failed(%d)\n", errno);
        fprintf(stderr, "stopping program...\n");
        return 1;
    }

    // setsockopt
    int no = 0;
    if (setsockopt(sfd, IPPROTO_IPV6, IPV6_V6ONLY, &no, sizeof(no)) != 0) {
        fprintf(stderr, "setsockopt(IPPROTP_IPV6, IPV6_V6ONLY) failed(%d)\n", errno);
        fprintf(stderr, "stopping program...\n");
        return 1;
    }
    int opt = 1;
    if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) != 0) {
        fprintf(stderr, "setsockopt(SOL_SOCKET, SO_REUSEADDR) failed(%d)\n", errno);
        fprintf(stderr, "stopping program...\n");
        return 1;
    }

    // bind
    if (bind(sfd, (struct sockaddr *)result->ai_addr, result->ai_addrlen) < 0) {
        fprintf(stderr, "bind() failed(%d)\n", errno);
        fprintf(stderr, "stopping program...\n");
        close(sfd);
        return 1;
    }

    // freeaddrinfo
    freeaddrinfo(result);

    // listen
    if (listen(sfd, BACKLOG) < 0) {
        fprintf(stderr, "listen() failed(%d)\n", errno);
        fprintf(stderr, "stopping program...\n");
        close(sfd);
        return 1;
    }

    // FD_SET(master)
    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(sfd, &rfds);
    int max_socket = sfd;

    fprintf(stdout, "waiting for connection...\n");

    bnx_connection_t *cons = NULL;
    while(keep_running) {
        // select
        fd_set rfds_copy = rfds;
        if (select(max_socket + 1, &rfds_copy, NULL, NULL, NULL) == -1) {
            fprintf(stderr, "select failed (%d)\n", errno);
            if (errno == EINTR) { // when got SIGINT, errno is set to be EINTR
                continue;
            } else {
                keep_running = 0;
                continue;
            }
        }

        if (FD_ISSET(sfd, &rfds_copy)) { // when getting new request from client.
            // TODO: check if connection count exceeds max-connection.
            int new_fd = accept(sfd, NULL, NULL);
            if (new_fd < 0) {
                fprintf(stderr, "accept() failed(%d)\n", errno);
                continue;
            }

            FD_SET(new_fd, &rfds);
            if (max_socket < new_fd) {
                max_socket = new_fd;
            }

            bnx_connection_t *new_connection = bnx_create_connection(new_fd);
            cons = bnx_prepend_connection(cons, new_connection);
            if (cons == NULL) {
                fprintf(stderr, "bnx_prepend_connection() failed\n");
                continue;
            }
        }

        bnx_connection_t *runner = cons;
        while ((runner != NULL) && keep_running) {
            if (FD_ISSET(runner->fd, &rfds)) {
                ssize_t byte_received = recv(
                    runner->fd,
                    runner->rbuf + runner->recieved,
                    BNX_REQUEST_BUF - runner->recieved - 1,
                    0
                );

                if (byte_received <= 0) {
                    fprintf(stderr, "receive() failed\n\terrno: %d\n\tbyte received: %ld\n", errno, byte_received);
                } else {
                    fprintf(stdout, "recieved message\n");
                    runner->recieved += byte_received;
                    runner->rbuf[runner->recieved + 1] = '\0';

                    char header_buf[SEND_BUF_LEN] = {0};
                    char body_buf[SEND_BUF_LEN] = {0};
                    ssize_t byte_sent = 0;
                    char *path = (argc == 4) ? argv[3] : "./index.html";
                    int byte_read = read_file_contents(body_buf, SEND_BUF_LEN, path);
                    if (byte_read <= 0) {
                        fprintf(stderr, "empty resource or request not found(read %d bytes)\n", byte_read);

                        const char *c404 = "HTTP/1.1 404 Not Found\r\n"
                            "Connection: close\r\n"
                            "Content-Length: 9\r\n\r\nNot Found";
                        send(runner->fd, c404, strlen(c404), 0);
                    } else {
                        const char *header = "HTTP/1.1 200 OK\r\n"
                            "Connection: close\r\n"
                            "Content-Length: %lu\r\n"
                            "Content-Type: text/html\r\n\r\n";
                        sprintf(header_buf, header, (unsigned long)byte_read);
                        byte_sent += send(runner->fd, header_buf, strlen(header_buf), 0);
                        byte_sent += send(runner->fd, body_buf, strlen(body_buf), 0);

                        fprintf(stdout, "sent %ld byte\n", byte_sent);
                    }
                }

                FD_CLR(runner->fd, &rfds);
                cons = bnx_drop_connection(&cons, runner);
                fprintf(stdout, "drop connection...\n");
            }
            runner = runner->next;
        } // while ((runner != NULL) && keep_running)
    } // while (keep_running)

    // free resource
    close(sfd);
    bnx_free_connection(&cons);
    fprintf(stdout, "Gracefully stopping...\n");
    return 0;
}
