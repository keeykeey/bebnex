#include "config.h"
#include "core.h"

bnx_uint_t bnx_str_len(char *ch)
{
    bnx_int_t l = 0;
    while(*ch++ != '\0')
    {
        l++;
    }
    return l;
}

bnx_string_t bnx_create_string(char *ch)
{
    bnx_string_t s;
    s.data = ch;
    s.pos = ch;
    s.length = bnx_str_len(s.data);

    return s;
}
