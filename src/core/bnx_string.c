#include "config.h"
#include "core.h"

char *bnx_copy_string(char *dst, char *src)
{
    char *ch = dst;
    while (*dst++ = *src++) {
        ;
    }
    *dst = '\0';

    return ch;
}

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
    /*
     * TODO: 'bnx_str_len(ch) +1' would be better but the program stops when running on windows platform.
     */
    int str_len = bnx_str_len(ch) + 2;
    char *tmp = calloc(str_len, sizeof(char));
    s.data = bnx_copy_string(tmp, ch);
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
