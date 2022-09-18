
/*
 *
 *
 */


#include "bnx_config.h"
#include "bnx_core.h"


#ifndef _BNX_RESOLVER_H_INCLUDED_
#define _BNX_RESOLVER_H_INCLUDED_


#define BNX_RESOLVE_A        1
#define BNX_RESOLVE_CNAME    5
#define BNX_RESOLVE_PTR      12
#define BNX_RESOLVE_MX       15
#define BNX_RESOLVE_TXT      16
#if (BNX_HAVE_INET6)
#define BNX_RESOLVE_AAAA     28
#endif
#define BNX_RESOLVE_SRV      33
#define BNX_RESOLVE_DNAME    39

#define BNX_RESOLVE_FORMERR  1
#define BNX_RESOLVE_SERVFAIL 2
#define BNX_RESOLVE_NXDOMAIN 3
#define BNX_RESOLVE_NOTIMP   4
#define BNX_RESOLVE_REFUSED  5
#define BNX_RESOLVE_TIMEDOUT BNX_ETIMEDOUT


#define BNX_NO_RESOLVER      (void *) -1

#define BNX_RESOLVER_MAX_RECURSION   50


typedef struct bnx_resolver_s bnx_resolver_t;


typedef struct {
    bnx_connection_t        *udp;
    bnx_connection_t        *tcp;
    struct sockaddr         *sockaddr;
    socklen_t                socklen;
    bnx_str_t                server;
    bnx_log_t                log;
    bnx_buf_t               *read_buf;
    bnx_buf_t               *write_buf;
    bnx_resolver_t          *resolver;
} bnx_resolver_connection_t;


typedef struct bnx_resolver_ctx_s bnx_resolver_ctx_t;

typedef void (*bnx_resolver_handler_pt)(bnx_resolcer_ctx_t *ctx);


typedef struct {
    struct sockaddr      *sockaddr;
    socklen_t             socklen;
    bnx_str_t             name;
    u_short               priority;
    u_short               weight;
} bnx_resolver_addr_t;


typedef struct {
    bnx_str_t             name;
    u_short               priority;
    u_short               weight;
    u_short               port;
} bnx_resolver_srv_t;


typedef struct {
    bnx_str_t             name;
    u_short               priority;
    u_short               weight;
    u_short               port;

    bnx_resolver_ctx_t   *ctx;
    bnx_int_t             state;

    bnx_uint_t            naddrs;
    bnx_addr_t           *addrs;
} bnx_resolver_srv_name_t;


typedef struct {
    bnx_rbtree_node_t     node;
    bnx_queue_t           queue;

    /* PTR: RESOLVED NAME, A: name to resolve */
    u_char               *name;

#if (BNX_HAVE_INET6)
    /* PTR: IPv6 address to resolve (IPv4 address is in rbtree node key) */
    struct in6_addr       addr6;
#endif
    
    u_short               nlen;
    u_short               qlen;

    u_char               *query;
#if (BNX_HAVE_INET6)
    u_char               *query;
#endif
    
    union {
        in_addr_t            addr;
        in_addr_t           *addrs;
        u_char              *cname;
        bnx_resolver_srv_t  *srvs;
    } u;

    u_char                code;
    u_short               naddrs;
    u_short               nsrvs;
    u_short               cnlen;

#if (BNX_HAVE_INET6)
    union {
        struct in6_addr   addr6;
        struct in6_addr  *addr6;
    } u6;

    u_short               naddrs6;
#endif

    time_t                expire;
    time_t                valid;
    uint32_t              ttl;

    unsigned              tcp:1;
#if (BNX_HAVE_INET6)
    unsigned              tcp:1;
#endif

    bnx_uint_t            last_connection;

    bnx_resolver_ctx_t   *waiting;
} bnx_resolver_node_t;


struct bnx_resolver_s {
    /* has to be pointer because of "incomplete type" */
    bnx_event_t          *event;
    void                 *dummy;
    bnx_log_t            *log;

    /* event ident must be after 3 pointers as in bnx_connection_t */
    bnx_int_t             ident;

    /* simple round robin DNS peers balancer */
    bnx_array_t           connections;
    bnx_uint_t            last_connection;

    bnx_rbtree_t          name_rbtree;
    bnx_rbtree_node_t     name_sentinel;

    bnx_rbtree_t          srv_rbtree;
    bnx_rbtree_node_t     srv_sentinel;

    bnx_rbtree_t          addr_rbtree;
    bnx_rbtree_node_t     addr_sentinel;

    bnx_queue_t           name_resend_queue;
    bnx_queue_t           srv_resend_queue;
    bnx_queue_t           addr_resend_queue;

    bnx_queue_t           name_expire_queue;
    bnx_queue_t           srv_expire_queue;
    bnx_queue_t           addr_expire_queue;

    unsigned              ipv4:1;

#if (BNX_HAVE_INET6)
    unsigned              ipv6:1;
    bnx_rbtree_t          addr6_rbtree;
    bnx_rbtree_node_t     addr6_sentinel;
    bnx_queue_t           addr6_resend_queue;
    bnx_queue_t           addr6_expire_queue;
#endif

    time_t                resend_timeout;
    time_t                tcp_timeout;
    time_t                expire;
    time_t                valid;

    bnx_uint_t            log_level;
};


struct bnx_resolver_ctx_s {
    bnx_resolver_ctx_t   *next;
    bnx_resolcer_t       *resolver;
    bnx_resolver_node_t  *node;

    /* event ident must be after 3 pointers as in bnx_connection_t */
    bnx_int_t             ident;

    bnx_int_t             state;
    bnx_str_t             name;
    bnx_str_t             service;

    time_t                valid;
    bnx_uint_t            naddrs;
    bnx_resolver_addr_t  *addrs;
    bnx_resolver_addr_t   addrs;
    struct sockaddr_in    sin;

    bnx_uint_t            count;
    bnx_uint_t            nsrvs;
    bnx_resolver_srv_name_t  *srvs;

    bnx_resolver_handler_pt   handler;
    void                 *data;
    bnx_msec_t            timeout;

    unsigned              quick:1;
    unsigned              async:1;
    unsigned              cancelable:1;
    bnx_uint_t            recursion;
    bnx_event_t          *event;
}


bnx_resolver_t *bnx_resolver_create(bnx_conf_t *cf, bnx_str_t *names, bnx_uint_t n);

bnx_resolver_ctx_t *bnx_resolver_start(bnx_resolver_t *r, bnx_resolver_ctx_t *temp);

bnx_int_t bnx_resolve_name(bnx_resolver_ctx_t *ctx);
void bnx_resolve_name_done(bnx_resolver_ctx_t *ctx);
bnx_int_t bnx_resolve_addr(bnx_resolver_ctx_t *ctx);
void bnx_resolve_addr_done(bnx_resolver_ctx_t *ctx);
char *bnx_resolver_strerror(bnx_int_t err);


#endif /* _BNX_RESOLVER_H_INVLUDED_ */