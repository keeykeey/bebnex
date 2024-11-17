#ifndef BNX_CONNECTION_H
#define BNX_CONNECTION_H

#include "./config.h"
#include "./core.h"

#define BUF_LEN 1024

typedef struct bnx_listening_s bnx_listening_t;
struct bnx_listening_s {
    bnx_socket_t fd;
    struct sockaddr *sockaddr;
    socklen_t socklen; // sizeof sockaddr
    size_t add_text_max_len;
    bnx_string_t addr_text;
    int type;  // SOCK_STREAM or SOCK_DGRAM
    int backlog;
    int rcvbuf;
    int sndbuf;
};

bnx_listening_t * bnx_create_listening(struct sockaddr *sockaddr, socklen_t socklen);
int bnx_bind_socket(bnx_socket_t fd, bnx_listening_t *ls, bnx_logger_t *errlogger);
int bnx_listen_socket(bnx_listening_t *ls, bnx_logger_t *errlogger);
int bnx_launch(bnx_listening_t *ls, bnx_conf_t conf, bnx_logger_t *accesslogger);

#endif /** BNX_CONNECTION_H */