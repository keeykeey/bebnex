#include "./test.h"

int main(void)
{
    int ng = 0;
    int count = 0;

    ng += test_bnx_connection(&count);
    ng += test_bnx_string(&count);
    ng += test_bnx_conf_file(&count);
    ng += test_bnx_log(&count);

    printf("###################### TEST RESULT ####################\n");
    printf("TOTAL: %d\nOK: %d\nNG: %d\n", count, count-ng, ng);
}
