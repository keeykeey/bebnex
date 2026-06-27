#include "core/bebnex.h"
#include "core/bnx_pool.h"
#include "string.h"

bnx_error_t bnx_pool_init(bnx_pool_t *pool, size_t size)
{
    if (!pool || size == 0) return bnx_error(BNX_ERROR, "Invalid argument");

    unsigned char *buf = (unsigned char *)malloc(size);
    if (!buf) return bnx_error(BNX_ERROR, "malloc failed");

    pool->buf = buf;
    pool->size = size;
    pool->start = (unsigned char *)buf;
    pool->end = (unsigned char *)buf + size;
    pool->current = bnx_align_ptr(pool->start, BNX_PTR_ALIGNMENT);
    pool->next = NULL;

    return bnx_success(BNX_OK);
}

bnx_error_t bnx_pool_destroy(bnx_pool_t *pool)
{
    if (!pool) return bnx_error(BNX_ERROR, "Invalid argument");

    if (pool->next) {
        bnx_pool_destroy(pool->next);
        pool->next = NULL;
    }

    if (pool->buf) {
        free(pool->buf);
        pool->buf = NULL;
    }

    pool->size = 0;
    pool->start = NULL;
    pool->end = NULL;
    pool->current = NULL;

    return bnx_success(BNX_OK);
}

bnx_error_t bnx_pool_reset(bnx_pool_t *pool)
{
    if (!pool) return bnx_error(BNX_ERROR, "Invalid argument");

    if (pool->start) {
        pool->current = bnx_align_ptr(pool->start, BNX_PTR_ALIGNMENT);
    }

    if(pool->next) {
        bnx_error_t result = bnx_pool_reset(pool->next);

        if (result.code != BNX_OK) {
            return result;
        }
    }

    return bnx_success(BNX_OK);
}

bnx_error_t bnx_pcalloc(bnx_pool_t *new_pool, bnx_pool_t *large_pool, size_t allocation_size)
{
    if (!large_pool || !new_pool || allocation_size == 0) return bnx_error(BNX_ERROR, "Invalid argument");

    // make sure new_pool is not pointing to other buffer
    memset(new_pool, 0, sizeof(*new_pool));

    unsigned char *aligned = bnx_align_ptr(large_pool->current, BNX_PTR_ALIGNMENT);

    // check for available space
    if ((aligned + allocation_size) > large_pool->end) return bnx_error(BNX_ERROR, "Memory error");

    new_pool->buf = aligned;
    new_pool->size = allocation_size;
    new_pool->start = aligned;
    new_pool->end = aligned + allocation_size - 1;
    new_pool->current = aligned;
    new_pool->next = NULL;

    // fill with 0 (calloc semantics)
    memset(new_pool->buf, 0, allocation_size);

    // advance large_pool->current. otherwise next allocation would be overlapped
    large_pool->current += allocation_size;

    return bnx_success(BNX_OK);
}

bnx_error_t bnx_pmalloc(bnx_pool_t *new_pool, bnx_pool_t *large_pool, size_t allocation_size)
{
    if (!new_pool || !large_pool || allocation_size == 0) return bnx_error(BNX_ERROR, "Invalid argument");

    // make sure new_pool is not pointing to other buffer
    memset(new_pool, 0, sizeof(*new_pool));

    unsigned char *aligned = bnx_align_ptr(large_pool->current, BNX_PTR_ALIGNMENT);

    if ((aligned + allocation_size) > large_pool->end) return bnx_error(BNX_ERROR, "memory error");

    new_pool->buf = aligned;
    new_pool->size = allocation_size;
    new_pool->start = aligned;
    new_pool->end = aligned + allocation_size - 1;
    new_pool->current = aligned;
    new_pool->next = NULL;

    // advance large_pool->current, otherwise next allocation would be overlapped
    large_pool->current += allocation_size;

    return bnx_success(BNX_OK);
}
