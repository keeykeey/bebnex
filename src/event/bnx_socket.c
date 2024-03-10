#include "../core/core.h"
#include "../core/config.h"

bnx_socket_t bnx_create_socket(int domain, int type, int protocol)
{
    bnx_socket_t sock;
    sock.domain = domain;
    sock.type = type;
    sock.protocol = protocol;

    sock.fd = socket(sock.domain, sock.type, sock.protocol);

    if (sock.fd == 0)
    {
        fprintf(stderr, BNX_ERROR_MESSAGE);
        exit(1);
    };

    return sock;
}

struct sockaddr_in bnx_create_sockaddr_in(int family, in_addr_t addr, int port )
{
    sockaddr_in_t sin;
    sin.sin_family = family;
    sin.sin_addr.s_addr = addr;
    sin.sin_port = htons(port);

    return sin;
}

int bnx_bind_socket(int fd, sockaddr_in_t sin)
{
    if (bind(fd, (struct sockaddr *)&sin, sizeof(sin)) < 0)
    {
        fprintf(stderr, BNX_ERROR_MESSAGE);
        close(fd);
        exit(1);
    };

    return BNX_OK;
}

int bnx_listen_socket(int fd, int max_con)
{
    if (listen(fd, max_con) < 0)
    {
        fprintf(stderr, BNX_ERROR_MESSAGE);
        close(fd);
        exit(1);
    }

    return BNX_OK;
};

int bnx_launch(bnx_socket_t sock, sockaddr_in_t sin, bnx_conf_t conf)
{
    char buffer[BUF_LEN];
    while(1)
    {
        printf("waiting for connection...\n");
        fflush(stdout);
        int address_length = sizeof(sin);
        int new_socket = accept(
            sock.fd, 
            (struct sockaddr *)&sin, 
            (socklen_t *)&address_length
        );

        read(new_socket, buffer, BUF_LEN);
        fflush(stdout);

        // TODO: refactor
        FILE *fp;
        char response[BUF_LEN];
        char ch;
        if ((fp = fopen(conf.prefix.data, "r")) == NULL) {
            fprintf(stderr, BNX_ERROR_MESSAGE);
        } else {
            int i = 0;
            while ((ch = fgetc(fp)) != EOF) {
                response[i++] = ch;
            }
        }

        write(new_socket, response, strlen(response));
        close(new_socket);
    };
};
