#pragma once
#include "core/bebnex.h"
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BNX_PTR_ALIGNMENT sizeof(uintptr_t)
#define bnx_align_ptr(p, a) (unsigned char *)(((uintptr_t) (p) + ((uintptr_t) a - 1)) & ~((uintptr_t) a - 1))

typedef struct bnx_pool_s bnx_pool_t;
struct bnx_pool_s {
    unsigned char *buf;
    size_t         size;
    unsigned char *start;
    unsigned char *end;
    unsigned char *current;
    bnx_pool_t    *next;
};

bnx_code_e bnx_pool_init(bnx_pool_t *pool, size_t size);
bnx_code_e bnx_pool_destroy(bnx_pool_t *pool);
bnx_code_e bnx_pool_reset(bnx_pool_t *pool);
bnx_code_e bnx_pcalloc(bnx_pool_t *new_pool, bnx_pool_t *large_pool, size_t allocation_size);
bnx_code_e bnx_pmalloc(bnx_pool_t *new_pool, bnx_pool_t *large_pool, size_t size);

#ifdef __cplusplus
}
#endif