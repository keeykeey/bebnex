#include "../src/core/core.h"
#include "./test.h"

int test_socket() {
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

int main(void)
{
    int ng = 0;
    int count = 0;

    ng += test_socket();
    count++;

    printf("###################### TEST RESULT ####################\n");
    printf("TOTAL: %d\nOK: %d\nNG: %d", count, count-ng, ng);
}
