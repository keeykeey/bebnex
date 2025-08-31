#include <stdio.h>
#include "core/bnx_log.h"

int main(void)
{
    BNX_LOG_FATAL("fatal(%d)", 1);
    BNX_LOG_ERROR("error(%d)", 1);
    BNX_LOG_INFO("info(%d)", 104);
    BNX_LOG_DEBUG("debug(%d)", 104);
    BNX_LOG_INFO("hello %s", "world");
    return 0;
}
