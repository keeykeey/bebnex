#include "./core.h"

int bnx_write_log(char *log_file_path, bnx_string_t log)
{
    FILE *fp;
    if ((fp = fopen(log_file_path, "a")) == NULL) {
      fprintf(stdout, "couldn't open %s", BNX_ACCESS_LOG_FILE);
      fclose(fp);
      return BNX_NG;
    } else {
      int ret;
      if ((ret = fwrite(log.data, sizeof(char), log.length, fp)) < 0) {
          fclose(fp);
          return BNX_NG;
      }
    }

    fclose(fp);
    return BNX_OK;
}

bnx_string_t
bnx_create_access_log_message()
{
  time_t now;
  time(&now);
  struct tm *timer = localtime(&now);
  char message[BNX_LOG_MESSAGE_LEN];
  bnx_memset(message, '\0', BNX_LOG_MESSAGE_LEN);

  int year = timer->tm_year + 1900;
  int month = timer->tm_mon + 1;
  int mday = timer->tm_mday;
  int hour = timer->tm_hour;
  int min = timer->tm_min;
  int sec = timer->tm_sec;
  int ret;
  snprintf(message, 38, "%d-%d-%d %d:%d:%d recieved access\n", year, month, mday, hour, min, sec);
  bnx_string_t lm = bnx_create_string(message);
  return lm;
}
