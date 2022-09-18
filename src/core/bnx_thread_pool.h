
/*
 *
 *
 */


#ifndef _BNX_THREAD_POOL_H_INCLUDED_
#define _BNX_THREAD_POOL_H_INCLUDED_


#include "bnx_config.h"
#include "bnx_core.h"
#include "../event/bnx_event.h"


struct bnx_thread_task_s {
    bnx_thread_task_t    *next;
    bnx_uint_t            id;
    void                 *ctx;
    void                (*handler)(void *data, bnx_log_t *log);
    bnx_event_t           event;
};


typedef struct bnx_thread_pool_s bnx_thread_pool_t;


bnx_thread_pool_t *bnx_thread_pool_add(bnx_conf_t *cf, bnx_str_t *name);
bnx_thread_pool_t *bnx_thread_pool_get(bnx_cycle_t *cycle, bnx_str_t *name);

bnx_thread_task_t *bnx_thread_task_alloc(bnx_pool_t *pool, size_t size);
bnx_int_t bnx_thread_task_post(bnx_thread_pool_t *tp, bnx_thread_task_t *task);


#endif /* _BNX_THREAD_POOL_H_INCLUDED_ */