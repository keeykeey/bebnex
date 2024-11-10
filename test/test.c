#include "./test.h"

int main(void)
{
    int ng = 0;
    int count = 0;

    fprintf(stdout, "###################### TEST RESULT ####################\n");

    ng += test_bnx_connection(&count);
    ng += test_bnx_string(&count);
    ng += test_bnx_conf_file(&count);
    ng += test_bnx_log(&count);

    fprintf(stdout, "\n[TOTAL]: %d\n[OK]: %d\n[NG]: %d\n", count, count-ng, ng);
    fprintf(stdout, "###################### TEST RESULT ####################\n");
}
