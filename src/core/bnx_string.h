#ifndef BNX_STRING_H
#define BNX_STRING_H

#include "core.h"

#define bnx_memzero(buf, n)     (void) memset(buf, 0, n) 
#define bnx_memset(buf, c, n)   (void) memset(buf, c, n)

typedef struct bnx_string_s {
    char             *data;
    bnx_uint_t        length;
} bnx_string_t;

char *bnx_copy_string(char *dst, char *src);
bnx_uint_t bnx_str_len(char* ch);
bnx_string_t bnx_create_string(char *ch);
bnx_uint_t bnx_atoui(bnx_string_t s);

#endif /** BNX_STRING_H */