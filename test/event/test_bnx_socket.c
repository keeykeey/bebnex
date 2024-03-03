#include "../../src/core/core.h"
#include "../../src/core/config.h"
//#include "../../src/event/bnx_socket.h"
#include "../test.h"

int test_bnx_create_socket() {
    bnx_socket_t sock = bnx_create_socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (sock.domain != PF_INET)
    {
        return BNX_TEST_NG;
    }

    if (sock.type != SOCK_STREAM)
    {
        return BNX_TEST_NG;
    }
    
    if (sock.protocol != IPPROTO_TCP)
    {
        return BNX_TEST_NG;
    }

    if (!sock.fd)
    {
        return BNX_TEST_NG;
    }

    return BNX_TEST_OK;
};

int test_bnx_socket(int *count)
{
    int ng = 0;
    ng += test_bnx_create_socket();
    (*count)++;

    return ng;
}