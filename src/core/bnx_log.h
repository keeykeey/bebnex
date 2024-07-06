#ifndef BNX_LOG_H
#define BNX_LOG_H

#include "./core.h"

#define BNX_LOG_MESSAGE_LEN 1024

int bnx_write_log(char *log_file_path, bnx_string_t log);
bnx_string_t bnx_create_access_log_message();

#endif /** BNX_LOG_H */