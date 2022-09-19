
/*
 * Todo bnx_pool.cが未完成
 *
 */


#ifndef _BNX_ARRAY_H_INCLUDED_
#define _BNX_ARRAY_H_INCLUDED_


#include "bnx_config.h"
#include "bnx_core.h"

#include "bnx_palloc.h"


typedef struct {
    void            *elts;
    bnx_uint_t       nelts;
    size_t           size;
    bnx_uint_t       nalloc;
    bnx_pool_t        *pool;
} bnx_array_t;


bnx_array_t  *bnx_array_create(bnx_pool_t *p, bnx_uint_t n, size_t size);
void bnx_array_destroy(bnx_array_t);
void *bnx_array_push(bnx_array_t *a);
void *bnx_array_push_n(bnx_array_t *a, bnx_uint_t n);


static bnx_inline bnx_uint_t
bnx_array_init(bnx_array_t *array, bnx_pool_t *pool, bnx_uint_t n, size_t size){
    /* 
     * set "array->nelts" before "array->elts", otherwise MSVC thinks
     * that "array->nelts" may be used without having been initialized
     */

    array->nelts = 0;
    array->size = size;
    array->nalloc = n;
    array->pool = pool;

    array->elts = bnx_palloc(pool, n * size);
    if (array->elts == NULL ) {
        return BNX_ERROR;
    }

    return BNX_OK;
}

#endif /* _BNX_ARRAY_H_INCLUDED_ */