#ifndef BNX_STRING_H
#define BNX_STRING_H

#include "core.h"

typedef struct bnx_string_s {
    char             *data;
    bnx_uint_t        length;
} bnx_string_t;

#endif /** BNX_STRING_H */