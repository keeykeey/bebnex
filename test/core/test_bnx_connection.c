#include "../../src/core/core.h"
#include "../../src/core/config.h"
#include "../../src/core/bnx_connection.h"
#include "../test.h"

int test_bnx_create_listening() {
    struct sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(8080);
    sin.sin_addr.s_addr = INADDR_ANY;
    bnx_listening_t *ls = bnx_create_listening((struct sockaddr *)&sin, sizeof(sin));

    if (ls->sockaddr != (struct sockaddr *)&sin) return BNX_TEST_NG;
    if (ls->socklen != sizeof(sin)) return BNX_TEST_NG;
    if (ls->addr_text.length != sizeof((struct sockaddr *)&sin)) return BNX_TEST_NG;
    if (ls->sockaddr->sa_family == AF_INET && ls->add_text_max_len != BNX_INET_ADDR_STRLEN) return BNX_TEST_NG;
    if (ls->sockaddr->sa_family == AF_INET) {
        if (ls->add_text_max_len != BNX_INET_ADDR_STRLEN) return BNX_ERROR;
    } else {
        if (ls->add_text_max_len != BNX_SOCKADDR_STRLEN) return BNX_ERROR;
    }
    if (ls->fd != -1) return BNX_TEST_NG;
    if (ls->type < 0) return BNX_TEST_NG;
    if (ls->backlog < 0) return BNX_TEST_NG;
    if (ls->rcvbuf != -1) return BNX_TEST_NG;
    if (ls->sndbuf != -1) return BNX_TEST_NG;

    return BNX_TEST_OK;
};

int test_bnx_connection(int *count)
{
    int ng = 0;

    if ( test_bnx_create_listening() == BNX_TEST_OK) {
        (*count)++;
    } else {
        ng++;
        (*count)++;
        fprintf(stdout, "[failed] test_bnx_create_listening()\n");
    }

    fprintf(stdout, "[done] test_bnx_connection\n");
    return ng;
}