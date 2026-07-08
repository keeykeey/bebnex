#pragma once
#include <stddef.h>

#define bnx_str_literal(literal) {(char *)literal, sizeof(literal) - 1}
#define bnx_set_str(bnx_string, literal) \
    do { \
        (bnx_string)->data = (char *)(literal); \
        (bnx_string)->len = sizeof(literal) -1; \
    } while (0)

typedef struct bnx_string_s bnx_string_t;
struct bnx_string_s {
    char *data;
    size_t len;
};
