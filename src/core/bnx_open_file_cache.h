
/*
 *
 *
 */


#include "bnx_config.h"
#include "bnx_core.h"


#ifndef _BNX_OPEN_FILE_CACHE_H_INCLUDED_
#define _BNX_OPEN_FILE_CACHE_H_INCLUDED_


#define BNX_OPEN_FILE_DIRECTIO_OFF BNX_MAX_OFF_T_VALUE


typedef struct {
    bnx_fd_t            fd;
    bnx_file_uniq_t     uniq;
    time_t              mtime;
    off_t               size;
    off_t               fs_size;
    off_t               directio;
    size_t              read_ahead;

    bnx_err_r           err;
    char               *failed;

    time_t              valid;

    bnx_uint_t          min_uses;

#if (BNX_HAVE_OPENAT)
    size_t              disable_symlinks_from;
    unsigned            disable_symlinks:2;
#endif 

    unsigned            test_dir:1;
    unsigned            test_only:1;
    unsigned            log:1;
    unsigned            errors:1;
    unsigned            events:1;

    unsigned            is_dir:1;
    unsigned            is_file:1;
    unsigned            is_link:1;
    unsigned            is_exec:1;
    unsigned            is_directio:1;
} bnx_open_file_info_t;


typedef struct bnx_cached_open_file_s bnx_cached_open_file_t;

struct bnx_cached_open_file_s {
    bnx_rbtree_node_t      node;
    bnx_queue_t            queue;

    u_char                *name;
    time_t                 created;
    time_t                 accessed;

    bnx_fd_t               fd;
    bnx_file_uniq_t        uniq;
    time_t                 mtime;
    off_t                  size;
    bnx_err_t              err;

    uint32_t               uses;

#if (BNX_HAVE_OPENAT)
    size_t                 disable_symlinks_from;
    unsigned               disable_symlinks:2;
#endif
    
    unsigned               count:24;
    unsigned               close:1;
    unsigned               use_event:1;
    
    unsigned               is_dir:1;
    unsigned               is_file:1;
    unsigned               is_link:1;
    unsigned               is_exec:1;
    unsigned               is_directio:1;

    bnx_event_t           *event;
};


typedef struct {
    bnx_rbtree_t          rbtree;
    bnx_rbtree_node_t     sentinel;
    bnx_queue_t           expire_queue;

    bnx_uint_t            current;
    bnx_uint_t            max;
    time_t                inactive;
} bnx_open_file_cache_t;


typedef struct {
    bnx_open_file_cache_t     *cache;
    bnx_cached_open_file_t    *file;
    bnx_uint_t                 min_uses;
    bnx_log_t                 *log;
} bnx_open_file_cache_cleanup_t;


typedef struct {

    /* bnx_connection_t stub to allow c->fd as event ident */
    void                      *data;
    bnx_event_t               *read;
    bnx_event_t               *write;
    bnx_fd_t                   fd;

    bnx_cached_open_file_t    *file;
    bnx_open_file_cached_t    *cache;
} bnx_open_file_cache_event_t;


bnx_open_file_cache_t *bnx_open_file_cache_init(bnx_pool_t *pool,
    bnx_uint_t max, time_t inactive);
bnx_int_t bnx_open_cached_file(bnx_open_file_cache_t *cache, bnx_str_t *name,
    bnx_open_file_info_t *of, bnx_pool_t *pool);


#endif /* BNX_OPEN_FILE_CACHE_H_INCLUDED */