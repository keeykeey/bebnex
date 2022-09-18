
/*
 *
 *
 */


#ifndef _BNX_TIMES_H_INCLUDED_
#define _BNX_TIMES_H_INCLUDED_


#include "bnx_config.h"
#include "bnx_core.h"


typedef struct {
    time_t        sec;
    bnx_uint_t    msec;
    bnx_int_t     gmtoff;
} bnx_time_t;


void bnx_time_init(void);
void bnx_time_update(void);
void bnx_time_sigsafe_update(void);
u_char *bnx_http_time(u_char *buf, time_t t);
u_char *bnx_http_cookie_time(u_char *buf, time_t t);
void bnx_gmtime(time_t t, bnx_tm_t *tp);

time_t bnx_next_time(time_t when);
#define bnx_next_time_n          "mktime()"


extern volatile bnx_time_t      *bnx_cached_time;

#define bnx_time()               bnx_cached_time->sec
#define bnx_timeofday()          (bnx_time_t *)bnx_cached_time

extern volatile bnx_str_t        bnx_cached_err_log_time;
extern volatile bnx_str_t        bnx_cached_http_time;
extern volatile bnx_str_t        bnx_cached_http_log_time;
extern volatile bnx_str_t        bnx_cached_http_log_iso8601;
extern volatile bnx_str_t        bnx_cached_syslog_time;

/*
 * milliseconds elapsed since some unspecified point in the past
 * and truncated to bnx_msec_t, used in event timers
 */
extern volatile bnx_msec_t       bnx_current_msec;


#endif /* _BNX_TIMES_H_INCLUDED_ */