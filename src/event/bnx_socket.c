#include "../core/core.h"

bnx_socket_t bnx_create_socket(int domain, int type, int protocol)
{
    bnx_socket_t sock;
    sock.domain = domain;
    sock.type = type;
    sock.protocol = protocol;

    sock.fd = socket(sock.domain, sock.type, sock.protocol);

    return sock;
}