#include "../../src/core/core.h"
#include "../../src/core/config.h"
#include "../test.h"

int test_bnx_copy_string()
{
    char *src = "hello bebnex";
    char dst[13] = {};
    char *new = bnx_copy_string(dst, src);

    int i;
    if (strcmp(new, src) == 0) {
        return BNX_TEST_OK;
    } else {
        return BNX_TEST_NG;
    }
}

int test_bnx_str_len()
{
    char *c = "hello bebnex";
    bnx_int_t len = bnx_str_len(c);

    if (len != 12) {
        printf("len%d", len);
        return BNX_TEST_NG;
    } else {
        return BNX_TEST_OK;
    }
}

int test_bnx_create_string()
{
    char *c =  "hello bebnex";
    bnx_string_t s = bnx_create_string(c);
    if (s.length != 12) {
        return BNX_TEST_NG;
    } else if (strcmp(s.data, c) != 0) {
        return BNX_TEST_NG;
    } else {
        return BNX_TEST_OK;
    }
}

int test_bnx_atoui()
{
    bnx_string_t s = bnx_create_string("321");
    bnx_uint_t n = bnx_atoui(s);
    if (n == 321) {
        return BNX_TEST_OK;
    } else {
        return BNX_TEST_NG;
    }

    bnx_string_t s2 = bnx_create_string("8080");
    bnx_uint_t n2 = bnx_atoui(s);
    if (n2 == 8080) {
        return BNX_TEST_OK;
    } else {
        return BNX_TEST_NG;
    }
}

int test_bnx_string(int *count)
{
    int ng = 0;
    ng += test_bnx_copy_string();
    (*count)++;

    ng += test_bnx_str_len();
    (*count)++;

    ng += test_bnx_create_string();
    (*count)++;

    ng += test_bnx_atoui();
    (*count)++;

    return ng;

}