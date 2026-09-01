#pragma once
#include <stdlib.h>
#include <stdint.h>
#include "core/bebnex.h"
#include "core/bnx_log.h"


#define BNX_POOL_ALIGNMENT 16
#define BNX_ALIGNMENT sizeof(uintptr_t)
#define bnx_align_ptr(p, a) (unsigned char *)(((uintptr_t) (p) + ((uintptr_t) (a) - 1)) & ~((uintptr_t) (a) - 1))


typedef struct bnx_pool_data_s bnx_pool_data_t;
typedef struct bnx_pool_s bnx_pool_t;
typedef struct bnx_pool_large_data_s bnx_pool_large_data_t;


struct bnx_pool_data_s {
    unsigned char   *last;
    unsigned char   *end;
    bnx_pool_data_t *next;
    int              failed;
};


struct bnx_pool_large_data_s {
    void                  *alloc;
    bnx_pool_large_data_t *next;
};


struct bnx_pool_s {
    bnx_pool_data_t        d;
    size_t                 dsize;
    bnx_pool_large_data_t *large;
    bnx_pool_data_t       *current;
    bnx_log_t             *log;
};


bnx_pool_t *bnx_create_pool(size_t size, bnx_log_t *log);
bnx_return_t bnx_pool_destroy(bnx_pool_t **pool);
bnx_return_t bnx_pool_reset(bnx_pool_t *pool);
void *bnx_pmalloc(bnx_pool_t *source, size_t size);
void *bnx_pcalloc(bnx_pool_t *source, size_t size);
