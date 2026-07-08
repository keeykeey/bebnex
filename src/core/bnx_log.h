#pragma once
#include <stdio.h>
#include "core/bebnex.h"
#include "core/bnx_string.h"

typedef enum bnx_log_level_e {
    BNX_LOG_LEVEL_CRIT   = 1,
    BNX_LOG_LEVEL_ERROR  = 2,
    BNX_LOG_LEVEL_INFO   = 3,
    BNX_LOG_LEVEL_DEBUG  = 4,
} bnx_log_level_t;

typedef struct bnx_log_s bnx_log_t;
typedef void (*bnx_log_writer_pt)(bnx_log_t *log, const char *buf, size_t size);

struct bnx_log_s {
    int                 log_level;
    int                 fd;
    bnx_string_t       *fpath;
    bnx_log_writer_pt   writer;
    bnx_log_t          *next;
};

bnx_return_t bnx_init_log(bnx_log_t *log, bnx_log_level_t log_level, bnx_string_t *fpath, bnx_log_writer_pt writer);
bnx_return_t bnx_open_log_file(bnx_log_t *log);
bnx_return_t bnx_close_log_file(bnx_log_t *log);
void bnx_log_writer(bnx_log_t *log, const char *msg, size_t size);
bnx_return_t bnx_write_logs(bnx_log_t *head, bnx_log_level_t log_level, size_t buf_size, const char *fmt, ...);
