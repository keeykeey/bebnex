#include "./core.h"

bnx_int_t bnx_init_logger(
  bnx_logger_t *log,
  bnx_int_t (*fwriter)(bnx_string_t log, bnx_logger_t *logger),
  char *fpath
) {
  log->fwriter = bnx_write_log;
  log->fpath = fpath;
  return BNX_OK;
}

bnx_int_t bnx_write_log(bnx_string_t log, bnx_logger_t *logger)
{
    FILE *fp;
    if ((fp = fopen(logger->fpath, "a")) == NULL) {
      if (log.data != NULL) {
        free(log.data);
      }

      return BNX_ERROR;

    } else {
      int ret;
      if ((ret = fwrite(log.data, sizeof(char), log.length, fp)) < 0) {
          fclose(fp);

          if (log.data != NULL) {
            free(log.data);
          }

          return BNX_ERROR;
      }

      if (log.data != NULL) {
        free(log.data);
      }

      fclose(fp);
    }

    return BNX_OK;
}

bnx_string_t
bnx_create_access_log_message()
{
  time_t now;
  time(&now);
  struct tm *timer = localtime(&now);
  char *message = (char *)calloc(BNX_LOG_MESSAGE_LEN, sizeof(char));

  int year = timer->tm_year + 1900;
  int month = timer->tm_mon + 1;
  int mday = timer->tm_mday;
  int hour = timer->tm_hour;
  int min = timer->tm_min;
  int sec = timer->tm_sec;
  int ret;
  snprintf(message, BNX_LOG_MESSAGE_LEN, "%d-%.2d-%.2d %.2d:%.2d:%.2d recieved access\n", year, month, mday, hour, min, sec);
  bnx_string_t lm = bnx_create_string(message);
  return lm;
}

bnx_string_t
bnx_create_error_log_message(char *log_message)
{
  time_t now;
  time(&now);
  struct tm *timer = localtime(&now);
  char *message = (char *)calloc(BNX_LOG_MESSAGE_LEN, sizeof(char));

  int year = timer->tm_year + 1900;
  int month = timer->tm_mon + 1;
  int mday = timer->tm_mday;
  int hour = timer->tm_hour;
  int min = timer->tm_min;
  int sec = timer->tm_sec;
  int ret;
  snprintf(message, BNX_LOG_MESSAGE_LEN, "%d-%.2d-%.2d %.2d:%.2d:%.2d [error] %s\n", year, month, mday, hour, min, sec, log_message);
  bnx_string_t lm = bnx_create_string(message);
  return lm;
}

bnx_string_t
bnx_vsnerrorlog(int size, char *fmt, ...)
{
  va_list arg;
  va_start(arg, fmt);
  char buf[size];
  vsnprintf(buf, size, fmt, arg);

  time_t now;
  time(&now);
  struct tm *timer = localtime(&now);
  char *message = (char *)calloc(BNX_LOG_MESSAGE_LEN, sizeof(char));

  int year = timer->tm_year + 1900;
  int month = timer->tm_mon + 1;
  int mday = timer->tm_mday;
  int hour = timer->tm_hour;
  int min = timer->tm_min;
  int sec = timer->tm_sec;
  int ret;
  snprintf(message, BNX_LOG_MESSAGE_LEN, "%d-%.2d-%.2d %.2d:%.2d:%.2d [error] %s", year, month, mday, hour, min, sec, buf);
  bnx_string_t lm = bnx_create_string(message);
  return lm;
}