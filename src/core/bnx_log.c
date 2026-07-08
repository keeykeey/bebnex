#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdarg.h>
#include "core/bnx_log.h"
#include "core/bebnex.h"

bnx_return_t bnx_init_log(
    bnx_log_t *log,
    bnx_log_level_t log_level,
    bnx_string_t *fpath,
    bnx_log_writer_pt writer
) {
    if ((log == NULL) || (fpath == NULL) || (writer == NULL)) {
        return bnx_error(BNX_ERROR, "invalid argument");
    }
    
    log->log_level = log_level;
    log->fd = -1;
    log->fpath = fpath;
    log->writer = writer;
    log->next = NULL;

    return bnx_success(BNX_OK);
}

bnx_return_t bnx_open_log_file(bnx_log_t *log)
{
    if (log == NULL || log->fpath == NULL || log->fpath->data == NULL) {
        return bnx_error(BNX_ERROR, "invalid argument");
    }

    if (log->fd >= 0) {
        return bnx_error(BNX_ERROR, "file is already open");
    }
    
    int fd = open(log->fpath->data, O_RDWR | O_CREAT | O_APPEND, 0666); // equivalent to "a+" in fopen
    if (fd < 0) {
        char msg[log->fpath->len + 32];
        snprintf(msg, sizeof(msg), "open file(%s) failed(%d)", log->fpath->data, errno);
        return bnx_error(BNX_ERROR, msg);
    }

    log->fd = fd;

    return bnx_success(BNX_OK);
}

bnx_return_t bnx_close_log_file(bnx_log_t *log)
{
    if (log == NULL) {
        return bnx_error(BNX_ERROR, "invalid argument");
    }
    
    if (log->fd < 0) {
        return bnx_error(BNX_ERROR, "negative number of file descriptor");
    }

    if (close(log->fd) < 0) {
        char msg[32];
        snprintf(msg, sizeof(msg), "close failed(%d)", errno);
        return bnx_error(BNX_ERROR, msg);
    }
    
    log->fd = -1;
    
    return bnx_success(BNX_OK);
}

void bnx_log_writer(bnx_log_t *log, const char *msg, size_t size)
{
    if (log == NULL || log->fd < 0 || msg == NULL) {
        return;
    }

    // Continue writing log after interupted by EINTR
    size_t written = 0;
    while (written < size) {
        ssize_t n = write(log->fd, msg + written, size - written);
        if (n < 0) {
            if (errno == EINTR) {
                continue;
            }
            break;
        }
        written += n;
    }
}

bnx_return_t bnx_write_logs(bnx_log_t *head, bnx_log_level_t log_level, size_t buf_size, const char *fmt, ...)
{
    if (head == NULL || fmt == NULL) {
        return bnx_error(BNX_ERROR, "invalid argument");
    }
    
    va_list args;
    va_start(args, fmt);

    char msg[buf_size];
    vsnprintf(msg, buf_size, fmt, args);
    
    bnx_log_t *runner = head;
    while (runner) {
        if (log_level <= runner->log_level) {
            runner->writer(runner, msg, buf_size);
        }

        runner = runner->next;
    }

    return bnx_success(BNX_OK);
}
