#ifndef BNX_LOG_H
#define BNX_LOG_H

#include "./core.h"

#define BNX_LOG_MESSAGE_LEN 1024

typedef struct bnx_logger_s bnx_logger_t;
struct bnx_logger_s {
    bnx_int_t (*fwriter)(bnx_string_t log, bnx_logger_t *logger);
    char *fpath;
};

bnx_int_t bnx_init_logger(bnx_logger_t *log, bnx_int_t (*fwriter)(bnx_string_t log_message, bnx_logger_t *logger), char *fpath);
bnx_int_t bnx_write_log(bnx_string_t log, bnx_logger_t *logger);
bnx_string_t bnx_create_access_log_message();
bnx_string_t bnx_create_error_log_message(char *log_message);
bnx_string_t bnx_vsnerrorlog(int size, char *fmt, ...);

#endif /** BNX_LOG_H */