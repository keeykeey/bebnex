#include "../../src/core/core.h"
#include "../../src/core/config.h"
#include "../test.h"

int test_bnx_read_conf()
{
    bnx_logger_t logger;
    bnx_init_logger(&logger, bnx_write_log, "./test/testfiles/test.log");
    // expected values
    bnx_uint_t port_expected = 8080;
    bnx_string_t prefix_expected = bnx_create_string("/test/testfiles/index.html\0");

    bnx_string_t path = bnx_create_string("./test/testfiles/test.conf\0");
    bnx_conf_t conf = bnx_read_conf(path, &logger);

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

    if (test_bnx_read_conf() == BNX_TEST_OK) {
        (*count)++;
    } else {
        ng++;
        (*count)++;
        fprintf(stdout, "[failed] test_bnx_read_conf()\n");
    }

    fprintf(stdout, "[done] test_bnx_conf_file\n");

    return ng;
}