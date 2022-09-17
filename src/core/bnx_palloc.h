
/*
 * 
 *    
 */


 #ifndef _BNX_PALLOC_H_INCLUDED_
 #define _BNX_PALLOC_H_INCLUDED_


#include "bnx_config.h"
#include "bnx_core.h"

#include "../os/unix/bnx_files.h"


/*
 * BNX_MAX_ALLOC_FROM_POOL should be (bnx_pagesize -1), i.e. 4095 on x86
 * On Windows NT it decreases a number of locked pages in a kernel
 */
#define BNX_MAX_ALLOC_FROM_POOL       (bnx_pagesize-1)

#define BNX_DEFAULT_POOL_SIZE         (16 * 1024)

#define BNX_POOL_ALIGNMENT            16
#define BNX_MIN_POOL_SIZE                                                                 \
    bnx_align((sizeof(bnx_pool_t)+2 * sizeof(bnx_pool_large_t)), BNX_POOL_ALIGNMENT), BNX_POOL_ALIGNMENT)


typedef void (*bnx_pool_cleanup_pt)(void *data);

typedef struct bnx_pool_cleanup_s bnx_pool_cleanup_t;


struct bnx_pool_cleanup_s {
    bnx_pool_cleanup_pt    handler;
    void                   *data;
    bnx_pool_cleanup_t     *next;
};


typedef struct bnx_pool_large_s bnx_pool_large_t;

struct bnx_pool_large_s {
    bnx_pool_large_t    *next;
    void                *alloc;
};


typedef struct {
    u_char              *last;
    u_char              *end;
    bnx_pool_t          *next;
    bnx_uint_t           failed;
} bnx_pool_data_t;


struct bnx_pool_s {
    bnx_pool_data_t      d;
    size_t               max;
    bnx_pool_t          *current;
    bnx_chain_t         *chain;
    bnx_pool_large_t    *large;
    bnx_pool_cleanup_t  *cleanup;
    bnx_log_t           *log;
};


typedef struct {
    bnx_fd_t             fd;
    u_char              *name;
    bnx_log_t           *log;
} bnx_pool_cleanup_file_t;


bnx_pool_t  *bnx_create_pool(size_t size, bnx_log_t *log);
void bnx_destroy_pool(bnx_pool_t *pool);
void bnx_reset_pool(bnx_pool_t *pool);

void *bnx_palloc(bnx_pool_t  *pool, size_t size);
void *bnx_pnalloc(bnx_pool_t *pool, size_t size);
void *bnx_pcalloc(bnx_pool_t *pool, size_t size);
void *bnx_pmemalign(bnx_pool_t *pool, size_t size, size_t alignment);
bnx_int_t bnx_pfree(bnx_pool_t  *pool, void *p);


bnx_pool_cleanup_t *bnx_pool_cleanup_add(bnx_pool_t *p, size_t size);
void bnx_pool_run_cleanup_file(bnx_pool_t *p, bnx_fd_t fd);
void bnx_pool_cleanup_file(void *data);
void bnx_pool_delete_file(void *data);


#endif /* _BNX_PALLOC_H_INCLIDED_ */