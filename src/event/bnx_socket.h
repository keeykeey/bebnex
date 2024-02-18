#ifndef BNX_SOCKET_H
#define BNX_SOCKET_H

#include "../core/core.h"
#define BUF_LEN 1024

typedef struct bnx_socket_s {
    bnx_int_t domain;
    bnx_int_t type;
    bnx_int_t protocol;

    int fd;
} bnx_socket_t;

bnx_socket_t bnx_create_socket(int domain, int type, int protocol);
sockaddr_in_t bnx_create_sockaddr_in(int fd, in_addr_t addr, int port);
int bnx_bind_socket(int fc, sockaddr_in_t sin);
int bnx_listen_socket(int fd, int max_con);
int bnx_launch(bnx_socket_t sock, sockaddr_in_t sin);

#endif /** BNX_SOCKET_H */
