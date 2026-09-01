#include "core/bebnex.h"
#include "core/bnx_pool.h"
#include "string.h"


static void *bnx_memalign(size_t align, size_t size, bnx_log_t *log)
{
    void *memptr = NULL;
    int err = posix_memalign(&memptr, align, size);
    if (err != 0) {
        bnx_write_logs(log, BNX_LOG_LEVEL_ERROR, 64, "posix_memalign failed(%d)", err);
        return NULL;
    }
    return memptr;
}


bnx_pool_t *bnx_create_pool(size_t size, bnx_log_t *log)
{
    if (size < sizeof(bnx_pool_t)) {
        bnx_write_logs(log, BNX_LOG_LEVEL_ERROR, 64, "too small size for bnx_create_pool");
        return NULL;
    }

    bnx_pool_t *p = (bnx_pool_t *)bnx_memalign(BNX_POOL_ALIGNMENT, size, log);
    if (!p) {
        return NULL;
    }

    p->d.last = (unsigned char *)p + sizeof(bnx_pool_t);
    p->d.end = (unsigned char *)p + size;
    p->d.next = NULL;
    p->dsize = size - sizeof(bnx_pool_t);
    p->large = NULL;
    p->current = &(p->d);
    p->log = log;

    return p;
}


bnx_return_t bnx_pool_destroy(bnx_pool_t **pool)
{
    if (!pool) return bnx_error(BNX_ERROR, "Invalid argument");

    for (bnx_pool_large_data_t *l = (*pool)->large; l; l = l->next) {
       if (l->alloc) {
           free(l->alloc);
       }
    }

    bnx_pool_data_t *chain = (*pool)->d.next;
    if (chain) {
        for (bnx_pool_data_t *runner = chain, *next; /** void */ ; runner = next) {
            next = runner->next;
            free(runner);

            if (next == NULL) {
                break;
            }
        }
    }

    free(*pool);
    *pool = NULL;

    return bnx_success(BNX_OK);
}


bnx_return_t bnx_pool_reset(bnx_pool_t *pool)
{
    if (!pool) return bnx_error(BNX_ERROR, "Invalid argument");

    for (bnx_pool_large_data_t *l = pool->large; l; l = l->next) {
        if (l->alloc) {
            free(l->alloc);
        }
    }

    for (bnx_pool_data_t *d = pool->d.next; d; d = d->next) {
        d->last = (unsigned char *)d + sizeof(bnx_pool_data_t);;
        d->failed = 0;
    }

    pool->d.last =  (unsigned char *)pool + sizeof(bnx_pool_t);
    pool->large = NULL;
    pool->current = &(pool->d);

    return bnx_success(BNX_OK);
}


static void *bnx_palloc_block(bnx_pool_t *pool, size_t want)
{
    size_t psize = (size_t) (pool->d.end - (unsigned char *)pool);
    unsigned char *m = bnx_memalign(BNX_POOL_ALIGNMENT, psize, pool->log);
    if (m == NULL) {
        return NULL;
    }

    bnx_pool_data_t *new = (bnx_pool_data_t *)m;
    new->end = m + psize;
    new->next = NULL;
    new->failed = 0;

    m = m + sizeof(bnx_pool_data_t);
    m = bnx_align_ptr(m, BNX_ALIGNMENT);
    new->last = m + want;

    bnx_pool_data_t *runner;
    for (runner = pool->current; runner->next; runner = runner->next) {
        if (runner->failed++ > 4) {
            pool->current = runner->next;
        }
    }
    runner->next = new;

    return m;
}


static void *bnx_palloc_small(bnx_pool_t *source, size_t size, int align)
{

    for (bnx_pool_data_t *d = source->current; d; d = d->next) {
        unsigned char *m;
        if (align) {
            m = bnx_align_ptr(d->last, BNX_ALIGNMENT);
        } else {
            m = d->last;
        }

        if ((d->end - m) >= size) {
            d->last = m + size;
            return m;
        }
    }

    return bnx_palloc_block(source, size);
}


static void *bnx_palloc_large(bnx_pool_t *pool, size_t size)
{
    void *m = malloc(size);
    if (m == NULL) {
        return NULL;
    }

    int n = 0;
    bnx_pool_large_data_t *large;
    for (large = pool->large; large; large = large->next) {
        if (large->alloc == NULL) {
            large->alloc = m;
            return m;
        }

        if (++n > 3) {
            break;
        }
    }

    large = bnx_palloc_small(pool, sizeof(bnx_pool_large_data_t), 1);
    if (large == NULL) {
        free(m);
        return NULL;
    }

    // push new large in front of the pool->large
    large->alloc = m;
    large->next = pool->large;
    pool->large = large;

    return m;
}


void *bnx_pmalloc(bnx_pool_t *source, size_t size)
{
    if (size < source->dsize) {
        return bnx_palloc_small(source, size, 1);
    }

    return bnx_palloc_large(source, size);
}


void *bnx_pcalloc(bnx_pool_t *source, size_t size)
{
    void *p = bnx_pmalloc(source, size);
    if (p) {
        memset(p, 0, size);
    }

    return p;
}
