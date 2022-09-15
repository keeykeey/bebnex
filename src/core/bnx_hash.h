
/*
 *
 *
 */


#ifndef _BNX_HASH_H_INCLUDED_
#define _BNX_HASH_H_INCLUDED_


#include "bnx_config.h"
#include "bnx_core.h"


typedef struct {
    void               *value;
    u_short             len;
    u_char              name[1];
} bnx_hash_elt_t;


typedef struct {
    bnx_hash_elt_t    **buckets;
    bnx_uint_t          size;
} bnx_hash_t;


typedef struct {
    bnx_hash_t          hash;
    void               *value;
} bnx_hash_wildcard_t;


typedef struct {
    bnx_str_t           key;
    bnx_uint_t          key_hash;
    void               *values;
} bnx_hash_key_t;


typedef bnx_uint_t (*bnx_hash_key_pt)(u_char *data, size_t len);


typedef struct {
    bnx_hash_t            hash;
    bnx_hash_wildcard_t  *wc_head;
    bnx_hash_wildcard_t  *wc_tail;
} bnx_hash_combined_t;


typedef struct {
    bnx_hash_t           *hash;
    bnx_hash_key_pt       key;
    
    bnx_uint_t            max_size;
    bnx_uint_t            bucket_size;

    char                 *name;
    bnx_pool_t           *pool;
    bnx_pool_t           *temp_pool;
} bnx_hash_init_t;


#define BNX_HASH_SMALL   1
#define BNX_HASH_LARGE   2

#define BNX_HASH_LARGE_ASIZE    16384
#define BNX_HASH_LARGE_HSIZE    10007

#define BNX_HASH_WILDCARD_KEY   1
#define BNX_HASH_READONLY_KEY   2


typedef struct {
    bnx_uint_t     hsize;
    
    bnx_pool_t    *pool;
    bnx_pool_t    *temp_pool;
    
    bnx_array_t    keys;
    bnx_array_t   *keys_hash;

    bnx_array_t    dns_wc_head;
    bnx_array_t   *dns_wc_head_hash;

    bnx_array_t    dns_wc_tail;
    bnx_array_t   *dnx_wc_tail_hash;
} bnx_hash_keys_arrays_t;


typedef struct bnx_table_elt_s bnx_table_elt_t;

struct bnx_table_elt_s {
    bnx_uint_t        hash;
    bnx_str_t         key;
    bnx_str_t         value;
    u_char           *lowcase_key;
    bnx_table_elt_t  *next;
};


void *bnx_hash_find(bnx_hash_t *hash, bnx_uint_t key, u_char *name, size_t len);
void *bnx_hash_find_wc_head(bnx_hash_wildcard_t *hwc, u_char *name, size_t len);
void *bnx_hash_find_wc_tail(bnx_hash_wildcard_t *hwc, u_char *name, size_t len);
void *bnx_hash_find_combined(bnx_hash_combined_t *hash, bnx_uint_t key,
    u_char *name, size_t len);

bnx_int_t bnx_hash_init(bnx_hash_init_t *hinit, bnx_hash_key_t *names,
    bnx_uint_t nelts);
bnx_int_t bnx_hash_wildcard_init(bnx_hash_init_t *hinit, bnx_hash_key_t *names,
    bnx_uint_t nelts);

#define bnx_hash(key,c) ((bnx_uint_t) key * 31 + c)
bnx_uint_t bnx_hash_key(u_char *data, size_t len);
bnx_uint_t bnx_hash_key_lc(u_char *data, size_t len);
bnx_uint_t bnx_hash_strlow(u_char *dst, u_char *src, size_t n);


bnx_int_t bnx_hash_keys_array_init(bnx_hash_keys_arrays_t *ha, bnx_uint_t type);
bnx_int_t bnx_hash_add_key(bnx_hash_keys_arrays_t *ha, bnx_str_t *key,
    void *value, bnx_uint_t flags);


#endif /* _BNX_HASH_H_INVLUDED_ */