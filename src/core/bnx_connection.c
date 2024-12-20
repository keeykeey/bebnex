#include "./core.h"
#include "./config.h"

bnx_listening_t *
bnx_create_listening(struct sockaddr *sockaddr, socklen_t socklen)
{
    size_t len;
    bnx_listening_t *ls = (bnx_listening_t *)calloc(1, sizeof(bnx_listening_t));
    u_char text[BNX_SOCKADDR_STRLEN];

    bnx_memzero(ls, sizeof(bnx_listening_t));

    ls->sockaddr = sockaddr;
    ls->socklen = socklen;
    len = sizeof(sockaddr);
    ls->addr_text.length = len;

    switch (ls->sockaddr->sa_family) {
        case AF_INET:
            ls->add_text_max_len = BNX_INET_ADDR_STRLEN;
            break;
        case AF_INET6:
            ls->add_text_max_len = BNX_INET6_ADDR_STRLEN;
            break;
        default:
            ls->add_text_max_len = BNX_SOCKADDR_STRLEN;
            break;
    }

    ls->addr_text.data = (char *)calloc(1, len);
    if (ls->addr_text.data == NULL) {
        return NULL;
    }
    memcpy(ls->addr_text.data, text, len);

    ls->fd = -1;
    ls->type = SOCK_STREAM;
    ls->backlog = BNX_LISTEN_BACKLOG;
    ls->rcvbuf = -1;
    ls->sndbuf = -1;

    return ls;
}

int bnx_bind_socket(bnx_socket_t fd, bnx_listening_t *ls, bnx_logger_t *errlogger)
{
    ls->fd = fd;
    if (bind(ls->fd, ls->sockaddr, ls->socklen) < 0)
    {
        int eno = bnx_get_socket_errno();
        errlogger->fwriter(bnx_vsnerrorlog(30, "bind() failed (%d)\n", eno), errlogger);
        bnx_close_socket(ls->fd);
        exit(1);
    };

    return BNX_OK;
}

int bnx_listen_socket(bnx_listening_t *ls, bnx_logger_t *errlogger)
{
    if (listen(ls->fd, ls->backlog) < 0)
    {
        int eno = bnx_get_socket_errno();
        errlogger->fwriter(bnx_vsnerrorlog(30, "listen() failed (%d)\n", eno), errlogger);
        bnx_close_socket(ls->fd);
        exit(1);
    }

    return BNX_OK;
};

int bnx_launch(bnx_listening_t *ls, bnx_conf_t conf, bnx_logger_t *accesslogger)
{
    char buffer[BUF_LEN];
    while(1)
    {
        fprintf(stdout, "waiting for connection...\n");
        fflush(stdout);
        int address_length = ls->add_text_max_len;
        bnx_socket_t new_socket = bnx_accept(ls, accesslogger);

        recv(new_socket, buffer, BUF_LEN, 0);
        fflush(stdout);

        // write access log
        accesslogger->fwriter(bnx_create_access_log_message(), accesslogger);

        // TODO: refactor
        FILE *fp;
        char response[BUF_LEN] = { "HTTP/1.1 200 OK\nContent-Type: text/html\n\n" };
        int HEADER_LEN = 41;
        char ch;
        if ((fp = fopen(conf.prefix.data, "r")) == NULL) {
            fprintf(stderr, "[error] failed to open index file %s\n", conf.prefix.data);
        } else {
            int i = HEADER_LEN;
            while (((ch = fgetc(fp)) != EOF) && (i < BUF_LEN)) {
                response[i++] = ch;
            }
        }

        send(new_socket, response, strlen(response), 0);
        bnx_close_socket(new_socket);
    };
};
