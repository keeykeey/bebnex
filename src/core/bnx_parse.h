
/*
 *
 *
 */


#ifndef _BNX_PARSE_H_INCLUDED_
#define _BNX_PARSE_H_INCLUDED_


#include "bnx_config.h"
#include "bnx_core.h"


ssize_t bnx_parse_size(bnx_str_t *line);
off_t bnx_parse_offset(bnx_str_t *line);
bnx_int_t bnx_parse_time(bnx_str_t *line, bnx_uint_t is_sec);


#endif /* _BNX_PARSE_H_INCLUDED_ */