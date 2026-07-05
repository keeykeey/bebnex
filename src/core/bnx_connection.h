#pragma once

#include "core/bnx_pool.h"
#include <sys/socket.h>
#include <stdbool.h>

#define BNX_REQUEST_BUF 4096
#define BNX_MAX_CONNECTIONS 1024         // TODO: set dynamically from configuration file.

typedef struct bnx_connection_s bnx_connection_t;
struct bnx_connection_s {
    int                     fd;
    socklen_t               addrlen;
    struct sockaddr_storage address;
    char                    *rbuf;       // request buffer
    int                     recieved;    // recieved data length
    bnx_connection_t        *next;
    bnx_pool_t              *connection_pool;
    bool                    linked;      // if connection is linked to chain.
};

bnx_connection_t *bnx_create_connection(int fd);
void bnx_free_connection(bnx_connection_t **c);
bnx_connection_t *bnx_prepend_connection(bnx_connection_t *chain, bnx_connection_t *new_conn);
bnx_connection_t *bnx_get_connection(bnx_connection_t *chain, int fd);
bnx_connection_t *bnx_drop_connection(bnx_connection_t **chain, bnx_connection_t *target);
