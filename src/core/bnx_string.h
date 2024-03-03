#ifndef BNX_STRING_H
#define BNX_STRING_H

#include "core.h"

typedef struct bnx_string_s {
    char             *data;
    char             *pos;
    bnx_uint_t        length;
} bnx_string_t;

bnx_string_t bnx_create_string(char *ch);
bnx_uint_t bnx_str_len(char* ch);

#endif /** BNX_STRING_H */