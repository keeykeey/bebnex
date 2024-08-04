#ifndef TEST_BNX_LOG_H
#define TEST_BNX_LOG_H

#include "../../src/core/core.h"
#include "../../src/core/config.h"
#include "../test.h"

int test_bnx_write_log();
int test_bnx_create_access_log_message();
int test_bnx_log(int *count);

#endif /** TEST_BNX_LOG_H */