#include "../../src/core/core.h"
#include "../../src/core/config.h"
#include "../test.h"

void init_test_log_file()
{
    FILE *fp;
    // clear log file
    fp = fopen("./test/testfiles/test.log", "w");
    fclose(fp);
}

int test_create_bnx_log()
{
    bnx_string_t log = bnx_create_string("testing create_bnx_log function...\n");

    if (bnx_write_log("./test/testfiles/test.log", log) == BNX_TEST_NG) {
        return BNX_TEST_NG;
    }

    FILE *fp;
    if ((fp = fopen("./test/testfiles/test.log", "r")) == NULL) {
        fprintf(stderr, "couldn't open ./testfiles/log\n");
        fclose(fp);
        return BNX_TEST_NG;
    } else {
        char *cbuf = calloc(36, sizeof(char));
        fread(cbuf, sizeof(char), 35, fp);
        if (strncmp(cbuf, log.data, 35) == 0) {
            fclose(fp);
            free(cbuf);
            return BNX_TEST_OK;
        }
        free(cbuf);
    }
    fclose(fp);
    return BNX_TEST_NG;
}

int test_bnx_create_access_log_message()
{
    bnx_string_t lm = bnx_create_access_log_message(); 
    if (*(lm.data) == '\0' || lm.length <= 0) {
        printf("log error\n");
        return BNX_TEST_NG;
    }
    return BNX_TEST_OK;
}

int test_bnx_log(int *count)
{
    init_test_log_file();
    int ng = 0;
    ng += test_create_bnx_log();
    ng += test_bnx_create_access_log_message();
    (*count)++;

    return ng;
}