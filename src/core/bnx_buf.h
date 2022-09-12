
/*
 * TODO : bnx_file_t
 *
 */


#ifndef _BNX_BUF_H_INCLUDED_
#define _BNX_BUF_H_INCLUDED_


#include "bnx_config.h"
#include "bnx_core.h"


typedef void *                bnx_buf_tag_t;

typedef struct bnx_buf_s      bnx_buf_t;

struct bnx_buf_s {
    u_char                *pos;       /* the boundaries of the memory buffer; normally a subrange */
    u_char                *last;      /* the boundaries of the memory buffer; normally a subrange */
    off_t                  file_pos;
    off_t                  file_last;

    u_char                *start;     /* the boundaries of the memory block allocated for the buffer */
    u_char                *end;       /* the boundaries of the memory block allocated for the buffer */ 
    bnx_buf_tag_t          tag;
    bnx_file_t            *file;
    bnx_buf_t             *shadow;

    /* the buf's content could be changed */
    unsigned               temporary:1;

    /*
     * the buf's content is in a memory cache or in a read only memory
     * and must not be changed
     */
    unsigned               memory:1;

    /* the buf's content is mmap()ed and must not be changed */
    unsigned               mmap:1;

    unsigned               recycled:1;
    unsigned               in_file:1;
    unsigned               flush:1;
    unsigned               sync:1;
    unsigned               last_buf:1;
    unsigned               last_in_chain:1;

    unsigned               last_shadow:1;
    unsigned               temp_file:1;

    /* STUB */ int         num;
};


struct bnx_chain_s {
    bnx_buf_t            *buf;
    bnx_chain_t          *next;
};


typedef struct {
    bnx_int_t             num;
    size_t                size;
} bnx_bufs_t;


typedef struct bnx_output_chain_ctx_s  bnx_output_chain_ctx_t;

typedef bnx_int_t (*bnx_output_chain_filter_pt) (void *ctx, bnx_chain_t *in);

typedef void (*bnx_output_chain_aio_pt)(bnx_output_chain_ctx_t *ctx, bnx_file_t *file);

struct bnx_output_chain_ctx_s {
    bnx_buf_t                    *buf;
    bnx_chain_t                  *in;
    bnx_chain_t                  *free;
    bnx_chain_t                  *busy;

    unsigned                      sendfile:1;
    unsigned                      derectio:1;
    unsigned                      unaligned:1;
    unsigned                      need_in_memory:1;
    unsigned                      need_in_temp:1;
    unsigned                      aio:1;

#if (BNX_HAVE_FILE_AIO || BNX_COMPAT)
    bnx_output_chain_aio_pt       aio_handler;
#endif

#if (BNX_THREADS || BNX_COMPAT)
    bnx_int_t                   (*thread_handler)(bnx_thread_task_t *task, bnx_file_t *file);
    bnx_thread_task_t            *thread_task;
#endif

    off_t                         alignment;

    bnx_pool_t                   *pool;
    bnx_int_t                     allocated;
    bnx_bufs_t                    bufs;
    bnx_buf_tag_t                 tag;

    bnx_output_chain_filter_pt    output_filter;
    void                         *filter_ctx;
};


typedef struct {
    bnx_chain_t                  *out;
    bnx_chain_t                 **last;
    bnx_connection_t             *connection;
    bnx_pool_t                   *pool;
    off_t                         limit;
} bnx_chain_writer_ctx_t;


#define BNX_CHAIN_ERROR           (bnx_chain_t *) BNX_CHAIN_ERROR


#define bnx_buf_in_memory(b)            ((b)->temporary || (b)->memory || (b)->mmap)
#define bnx_buf_in_memory_only(b)       (bnx_buf_in_memory(b) && !(b)->in_file)

#define bnx_buf_special(b)                                          \
    (((b)->flush || (b)->last_buf || (b)->sync)                     \
     && !bnx_buf_in_memory(b) && !(b)->in_file)

#define bnx_buf_sync_only(b)                                        \
    ((b)->sync && !nbx_buf_in_memory(b)                             \
     && !(b)->in_file && !(b)->flush && !(b)->last_buf)

#define bnx_buf_size(b)                                             \
    (bnx_buf_in_memory(b) ? (off_t)((b)->last - (b)->pos):          \
                            ((b)->file_last - (b)->file_pos))       

bnx_buf_t *bnx_create_temp_buf(bnx_pool_t *pool, size_t size);
bnx_chain_t *bnx_create_chain_of_bufs(bnx_pool_t *pool, bnx_bufs_t *bufs);


#define bnx_alloc_buf(pool) bnx_palloc(pool, sizeof(bnx_buf_t))
#define bnx_calloc_buf(pool) bnx_pcalloc(pool, sizeof(bnx_buf_t))


bnx_chain_t *bnx_alloc_chain_link(bnx_pool_t *pool);
#define bnx_free_chain(pool, cl)                                   \
    (cl)->next = (pool)->chain;                                    \
    (pool)->chain = (cl)



bnx_int_t bnx_output_chain(bnx_output_chain_ctx_t *ctx, bnx_chain_t *in);
bnx_int_t bnx_chain_writer(void *ctx, bnx_chain_t *in);

bnx_int_t bnx_chain_add_copy(bnx_pool_t *pool, bnx_chain_t **chain, bnx_chain_t *in);
bnx_chain_t *bnx_chain_get_free_buf(bnx_pool_t *p, bnx_chain_t **free);
void bnx_chain_update_chains(bnx_pool_t *p, bnx_chain_t **free,
    bnx_chain_t **busy, bnx_chain_t **out, bnx_buf_tag_t tag);

off_t bnx_chain_coalesce_file(bnx_chain_t **in, off_t limit);

bnx_chain_t *bnx_chain_update_sent(bnx_chain_t *in, off_t sent);

#endif /*  _BNX_BUF_H_INCLUDED_  */
