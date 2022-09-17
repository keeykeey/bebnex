
/*
 *
 *
 */


#ifndef _BNX_PARSE_TIME_H_INCLUDED_
#define _BNX_PARSE_TIME_H_INCLUDED_


#include "bnx_config.h"
#include "bnx_core.h"


time_t bnx_parse_http_time(u_char *value, size_t len);

/* compatibility */
#define bnx_http_parse_time(value, len) bnx_parse_http_time(value, len)


#endif /* _BNX_PARSE_TIME_H_INCLUDED_ */