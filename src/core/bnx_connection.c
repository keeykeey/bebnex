#include "core/bnx_connection.h"
#include <stdlib.h>

bnx_connection_t *bnx_create_connection(int fd)
{
    if (fd < 0) {
        return NULL;
    }

    // TODO: create buffer from c->connection_pool
    bnx_connection_t *c = (bnx_connection_t *)calloc(1, sizeof(bnx_connection_t));
    if (!c) {
        return NULL;
    }

    c->fd = fd;
    c->addrlen = sizeof(c->address);
    c->rbuf = (char *)calloc(BNX_REQUEST_BUF, sizeof(char));
    if (!c->rbuf) {
        free(c);
        return NULL;
    }

    c->recieved = -1;
    c->next = NULL;
    c->connection_pool = NULL; // TODO
    c->linked = false;

    return c;
}


void bnx_free_connection(bnx_connection_t **c)
{
    if (*c == NULL) {
        return;
    }

    if ((*c)->rbuf != NULL) {
        free((*c)->rbuf);
    }

    if ((*c)->next != NULL) {
        bnx_free_connection(&(*c)->next);
    }

    // TODO free (*c)->connection_pool

    free(*c);
}

bnx_connection_t *bnx_prepend_connection(bnx_connection_t *chain, bnx_connection_t *new_conn)
{
    if (new_conn == NULL) {
        // if chain is also NULL, it returns NULL;
        return chain;
    }
    if (new_conn->linked) {
        return chain;
    }

    // set linked flag true to both of the linking connections
    new_conn->linked = true;
    if (chain != NULL) {
        chain->linked = true;
    }

    // if chain is NULL, new_conn->next is set as NULL.
    new_conn->next = chain;
    return new_conn;
}


bnx_connection_t *bnx_get_connection(bnx_connection_t *chain, int fd)
{
    if (chain == NULL) {
        return NULL;
    }

    bnx_connection_t *current = chain;
    bnx_connection_t *runner = chain->next;  // to prevent endless loop, use runner to detect circulation

    int counter = BNX_MAX_CONNECTIONS;  // TODO: get max connection dynamically
    while (current != NULL) {
        if (current->fd == fd) {
            return current;
        }
        current = current->next;

        if (--counter < 0) {
            // TODO: log alert
            return NULL;
        }
    }

    return NULL;
}


bnx_connection_t *bnx_drop_connection(bnx_connection_t **chain, bnx_connection_t *target)
{
    if (*chain == NULL) {
        return target;
    }

    if (target == NULL) {
        return *chain;
    }

    bnx_connection_t **current = chain;
    while (*current != NULL) {
        if (*current == target) {
            target->linked = false;
            *current = (*current)->next;
            if (target->rbuf != NULL) {
                free(target->rbuf);
            }
            if (target->connection_pool != NULL) {
                // TODO: free connection_pool)
            }

            free(target);
            continue;
        }
        current = &(*current)->next;
    }

    return *chain;
}
