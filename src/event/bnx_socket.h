#ifndef BNX_SOCKET_H
#define BNX_SOCKET_H

#include "../core/core.h"

typedef struct bnx_socket_s {
    bnx_int_t domain;
    bnx_int_t type;
    bnx_int_t protocol;

    int fd;
} bnx_socket_t;

bnx_socket_t bnx_create_socket(int domain, int type, int protocol);

#endif /** BNX_SOCKET_H */
