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

bnx_uint_t bnx_atoui(bnx_string_t s)
{
    bnx_uint_t n = 0;
    for (bnx_uint_t i = 0; i < s.length; i++)
    {
        bnx_uint_t add = (s.data[i] - '0');

        bnx_uint_t multi = s.length -1 - i;
        for (bnx_uint_t j = 0; j < multi; j++ )
        {
            add *= 10;
        }

        n += add;
    }

    return n;
}
