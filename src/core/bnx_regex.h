
/*
 *
 *
 */


#ifndef _BNX_REGEX_H_INCLUDED_
#define _BNX_REGEX_H_INCLUDED_


#include "bnx_config.h"
#include "bnx_core.h"


#if (BNX_PCR22)

#define PCRE2_CORE_UNIT_WIDTH 8
#include "pcre2.h"

#define BNX_REGEX_NO_MATCHED PCRE2_ERROR_NOMATCH /* -1 */

typedef pcre2_code bnx_regex_t;

#else

#include "pcre.h"

#define BNX_REGEX_NO_MATCHED PCRE_ERROR_NOMATCH /* -1 */

typedef struct {
    pcre          *code;
    pcre_extra    *extra;
} bnx_regex_t;

#endif


#define BNX_REGEX_CASELESS     0x00000001
#define BNX_REGEX_MULTILINE    0x00000002


typedef struct {
    bnx_str_t      pattern;
    bnx_pool_t    *pool;
    bnx_uint_t     options;

    bnx_regex_t   *regex;
    int            captures;
    int            named_captures;
    int            name_size;
    u_char        *names;
    bnx_str_t      err;
} bnx_regex_compule_t;


typedef struct {
    bnx_regex_t   *regex;
    u_char        *name;
} bnx_regex_elt_t;


void bnx_regex_init(void);
bnx_int_t bnx_regex_compile(bnx_regex_compile_t *rc);

bnx_int_t bnx_regex_exec(bnx_regex_t *re, bnx_str_t *s, int *captures, bnx_uint_t size);


#if (BNX_PCRE2)
#define bnx_regex_exec_n    "pcre2_match()"
#else
#define bnx_regex_exec_n    "pcre_exec()"
#endif

bnx_int_t bnx_regex_exec_array(bnx_array_t *a, bnx_str_t *s, bnx_log_t *log);


#endif /* _BNX_REGEX_H_INCLUDED_ */