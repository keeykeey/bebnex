#include "../../src/core/core.h"
#include "../../src/core/config.h"
#include "../test.h"

int test_bnx_read_conf()
{
    // expected values
    bnx_uint_t port_expected = 8080;
    bnx_uint_t max_con_expected = 10;
    bnx_string_t prefix_expected = bnx_create_string("/root_dev/var/www/index.html\0");

    bnx_string_t path = bnx_create_string("./test/testfiles/test.conf\0");
    bnx_conf_t conf = bnx_read_conf(path);

    if (conf.port != port_expected) {
        return BNX_TEST_NG;
    }

    if (strcmp(conf.prefix.data, prefix_expected.data) != 0) {
        return BNX_TEST_NG;
    }

    return BNX_TEST_OK;
}

int test_bnx_conf_file(int *count)
{

    int ng = 0;

    ng += test_bnx_read_conf();
    (*count)++;

    return ng;
}