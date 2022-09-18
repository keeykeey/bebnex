
/*
 * 
 *
 */


#ifndef _BNX_STRING_H_INCLUDED
#define _BNX_STRING_H_INCLUDED


#include "bnx_config.h"
#include "bnx_core.h"


typedef struct {
    size_t len;
    u_char *data;
}bnx_str_t;


typedef struct {
    bnx_str_t key;
    bnx_str_t value;
} bnx_keyval_t;


typedef struct{
    unsigned len:28;

    unsigned valid:1;
    unsigned no_cacheable:1;
    unsigned not_found:1;
    unsigned escape:1;

    u_char *data;
} bnx_variable_value_t;


#define bnx_string(str)  {sizeof(str)-1, (u_char *) str}
#define bnx_null_string  {0,NULL}
#define bnx_str_set(str, text)                                     \
    (str)->len = sizeof(text)-1; (str)->data = (u_char *) text
#define bnx_str_null(str)  (str)->len = 0; (str)->data=NULL


#define bnx_tolower(c)  (u_char)((c>='A' && c<='Z') ? (c | 0x20) : c)
#define bnx_toupper(c)  (u_char)((c>='a' && c<='z') ? (c & ~0x20) : c)

void bnx_strlow(u_char *dst, u_char *src, size_t n);


#define bnx_strncmp(s1,s2,n)  strncmp((const char *) s1, (const char *) s2)


/* msvc and icc7 compile strcmp() to inline loop */
#define bnx_strcmp(s1,s2) strcmp((const char *) s1, (const char *) s2)


#define bnx_strstr(s1,s2) strstr((const char *) s1, (const char *) s2)
#define bnx_strlen(s)     strlen((const char *) s)

size_t bnx_strnlen(u_char *p, size_t n);

#define bnx_strchr(s1,c)  strchr((const char *) s1, (int) c)

static bnx_inline u_char *
bnx_strlchr(u_char *p, u_char *last, u_char c){

    while (p < last) {

        if (*p==c){
            return p;
        }

        p++;
    }

    return NULL;
}


/*
 * msv and icc7 compile memset() to the inline "rep stos"
 * while ZeroMemory() and bzero() are the calls.
 * icc7 may also inlone several mov's of a zeroed register for amall blocks.
 */
#define bnx_memzero(buf,n)   (void)memset(buf,0,n)
#define bnx_memset(buf,c,n)  (void)memset(buf,c,n)

void bnx_explicit_memzero(void *buf, size_t n);


#if (BNX_MEMCPY_LIMIT)

void *bnx_memcpy(void *dst, const void *src, size_t n);
#define bnx_cpymem(dst,src,n)  (((u_char *) bnx_memcpy(dst,src,n))+(n))

#else

/*
 * gcc3,msvc,and icc7 compile memcpy() to the inline "rep movs"
 * gcc3 compiles memcpy(d,s,4) to the inline "mov"es,
 * icc8 compile memcpy(d,s,4) to the inline "mov"es or XMM moves.
 */
#define bnx_memcpy(dst,src,n) (void)memcpy(dst,src,n)
#define bnx_cpymem(dst,src,n) (((u_char *)memcpy(dst,src,n)) + (n))

#endif


#if (__INTEL_COMPILER >= 800)

/*
 * the simple inline cycle copies the cariable length strings up to 16
 * bytes faster than icc8 autodetecting _intel_fast_memcpy()
 */

static bnx_inline u_char *
bnx_copy(u_char *dst, u_char *src, size_t len){

    if (len <17 ){

        while(len){
            *dst++ = *src++;
            len--;
        }

        return dst;

    } else{
        return bnx_cpymem(dst,src,len);
    }
}

#else

#define bnx_copy bnx_cpymem

#endif


#define bnx_memmove(dst, src, n)  (void) memmove(dst,src,n)
#define bnx_movemem(dst, src, n)  (((u_char *) memmove(dst,src,n)) + (n))


/* msvc and icc7 compile memcmp() to the inline loop */
#define bnx_memcmp(s1,s2,n)  memcmp((const char *) s1, (const char *) s2, n)


u_char *bnx_coystrn(u_char *dst, u_char *src, size_t n);
u_char *bnx_pstrdup(bnx_pool_t *pool, bnx_str_t *src);
u_char * bnx_cdecl bnx_sprintf(u_char *buf, const char *fmt, ...);
u_char * bnx_cdecl bnx_snprintf(u_char *buf, size_t max, const char *fmt, ...);
u_char * bnx_cdecl bnx_slprintf(u_char *buf, u_char *last, const char *fmt,...);

u_char *bnx_vslprintf(u_char *buf, u_char *last, const char *fmt,va_list args);
#define bnx_vsnprintf(buf, max, fmt, args)  bnx_vslprintf(buf, buf+(max), fmt, args)


bnx_int_t bnx_strcasecmp(u_char *s1, u_char *s2);
bnx_int_t bnx_strncasecmp(u_char *s1, u_char *s2, size_t n);

u_char *bnx_strnstr(u_char *s1, char *s2, size_t n);

u_char *bnx_strstrn(u_char *s1, char *s2, size_t n);
u_char *bnx_strcasestrn(u_char *s1, char *s2, size_t n);
u_char *bnx_strlcasestrn(u_char *s1, u_char *last, u_char *s2, size_t n);

bnx_int_t bnx_rstrncmp(u_char *s1, u_char *s2, size_t n);
bnx_int_t bnx_rstrncasecmp(u_char *s1, u_char *s2, size_t n);
bnx_int_t bnx_memn2cmp(u_char *s1, u_char *s2, size_t n1, size_t n2);
bnx_int_t bnx_dns_strcmp(u_char *s1, u_char *s2);
bnx_int_t bnx_filename_cmp(u_char *s1, u_char *s2, size_t n);

bnx_int_t bnx_atoi(u_char *line, size_t n);
bnx_int_t bnx_atofp(u_char *line, size_t n, size_t point);
ssize_t bnx_atosz(u_char *line, size_t n);
off_t bnx_atoof(u_char *line, size_t n);
time_t bnx_atotm(u_char *line, size_t n);
bnx_int_t bnx_hextoi(u_char *line, size_t n);

u_char *bnx_hex_dump(u_char *dst, u_char *src, size_t len);


#define bnx_base64_encoded_length(len)  (((len +2) / 3) * 4)
#define bnx_base64_decoded_length(len)  (((len +3) / 4) * 3)

void bnx_encode_base64(bnx_str_t *dst, bnx_str_t *src);
void bnx_encode_base64url(bnx_str_t *dst, bnx_str_t *src);
bnx_int_t bnx_decode64(bnx_str_t *dst, bnx_str_t *src);
bnx_int_t bnx_decode64url(bnx_str_t *dst, bnx_str_t *src);

uint32_t bnx_utf8_decode(u_char **p, size_t n);
size_t bnx_utf8_length(u_char *p, size_t n);
u_char *bnx_utf8_cpystrn(u_char *dst, u_char *src, size_t n, size_t len);


#define BNX_ESCAPE_URI             0
#define BNX_ESCAPE_ARGS            1
#define BNX_ESCAPE_URI_COMPONENT   2
#define BNX_ESCAPE_HTML            3
#define BNX_ESCAPE_REFRESH         4
#define BNX_ESCAPE_MEMCACHED       5
#define BNX_ESCAPE_MAIL_AUTH       6

#define NGX_UNESCAPE_URI        1
#define NGX_UNESCAPE_REDIRECT   2

uintptr_t bnx_escape_uri(u_char *dst, u_char *src, size_t size, bnx_uint_t type);

void bnx_unescaoe_url(u_char **dst, u_char **src, size_t size, bnx_uint_t type);
uintptr_t bnx_escape_html(u_char *dst, u_char *src, size_t size);
uintptr_t bnx_escape_json(u_char *dst, u_char *stc, size_t size);


typedef struct {
    bnx_rbtree_node_t   node;
    bnx_str_t           str;
} bnx_str_node_t;


void bnx_str_rbtree_insert_value(bnx_rbtree_node_t *tmp, 
    bnx_rbtree_node_t *node, bnx_rbtree_node_t *sentinel);
bnx_str_node_t *bnx_str_rbtree_lookup(bnx_rbtree_t *rbtree, bnx_str_t *name,
    uint32_t hash);


void bnx_sort(void *base, size_t n, size_t size,
    bnx_int_t (*cmp)(const void *, const void *));
#define bnx_qsort qsort


#define bnx_value_helper(n)  #n
#define bnx_value(n) bnx_value_helper(n)


#endif /* _BNX_STRING_H_INCLUDED */