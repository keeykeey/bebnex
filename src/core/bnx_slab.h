
/*
 *
 *
 */


#ifndef _BNX_SLAB_H_INCLUDED_
#define _BNX_SLAB_H_INCLUDED_


#include "bnx_config.h"
#include "bnx_core.h"


typedef struct bnx_slab_page_s bnx_slab_page_t;

struct bnx_slab_page_s {
    uintptr_t         slab;
    bnx_slab_page_t  *next;
    uintptr_t        *prev;
};


typedef struct {
    bnx_uint_t        total;
    bnx_uint_t        used;

    bnx_uint_t        regs;
    bnx_uint_t        fails;
} bnx_slab_stat_t;


typedef struct {
    bnx_shmtx_sh_t    lock;

    size_t            min_size;
    size_t            min_shift;

    bnx_slab_page_t  *pages;
    bnx_slab_page_t  *last;
    bnx_slab_page_t   free;

    bnx_slab_stat_t  *stats;
    bnx_uint_t        pfree;

    u_char           *start;
    u_char           *end;

    bnx_shmtx_t       mutex;

    u_char           *log_ctx;
    u_char            zero;

    unsigned          log_nomem:1;

    void             *data;
    void             *addr;
} bnx_slab_pool_t;


void bnx_slab_sizes_init(void);
void bnx_slab_init(bnx_slab_pool_t *pool, size_t size);
void *bnx_slab_alloc(bnx_slab_pool_t *pool, size_t size);
void *bnx_slab_alloc_locked(bnx_slab_pool_t *pool, size_t size);
void *bnx_slab_calloc(bnx_slab_pool_t *pool, size_t size);
void *bnx_slab_calloc_locked(bnx_slab_pool_t *pool, size_t size);
void bnx_slab_free(bnx_slab_pool_t *pool, void *p);
void bnx_slab_free_locked(bnx_slab_pool_t *pool, void *p);


#endif /* _BNX_SLAB_H_INCLUDED_ */