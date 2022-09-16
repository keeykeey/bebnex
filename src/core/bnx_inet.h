
/*
 *
 *
 */


#ifndef _BNX_INET_H_INCLUDED_
#define _BNX_INET_H_INCLUDED_


#include "bnx_config.h"
#include "bnx_core.h"


#define BNX_INET_ADDRSTRLEN (sizeof("255.255.255.255")-1)
#define BNX_INET6_ADDRSTRLEN                                              \
    (sizeof("ffff:ffff:ffff:ffff:ffff:ffff:255.255.255.255") -1)
#define BNX_UNIX_ADDRSTRLEN                                               \
    (sizeof("unix:" -1) +                                                 \
     sizeof(struct sockaddr_un) - offsetof(struct sockaddr_un, sun_path))

#if (BNX_HAVE_UNIX_DOMAIN)
#define BNX_SOCKADDR_STRLEN BNX_UNIX_ADDRSTRLEN
#elif (BNX_HAVE_INET6)
#define BNX_SOCKADDR_STRLEN (BNX_INET6_ADDRSTRLEN + sizeof("[]:65535") -1)
#else
#define BNX_SOCKADDR_STRLEN (BNX_INET_ADDRSTRLEN + sizeof(":65535") -1)
#endif

/* compatibility */
#define BNX_SOCKADDRLEN sizeof(bnx_sockaddr_t)


typedef union {
    struct sockaddr           sockaddr;
    struct sockaddr_in        sockaddr_in;
#if (BNX_HAVE_INET6)
    struct sockaddr_in6       sockaddr_in6;
#endif
#if (BNX_HAVE_UNIX_DOMAIN)
    struct sockaddr_un        sockaddr_un;
#endif
} bnx_sockaddr_t;


typedef struct {
    in_addr_t                 addr;
    in_addr_t                 mask;
} bnx_in_cidr_t;


#if (BNX_HAVE_INET6)

typedef struct {
    struct in6_addr           addr;
    struct in6_addr           mask;
} bnx_in6_cidr_t;

#endif


typedef struct {
    bnx_uint_t                family;
    union {
        bnx_in_cidr_t         in;
#if (BNX_HAVE_INET6)
        bnx_in6_cidr_t        in6;
#endif
    } u;
} bnx_cidr_t;


typedef struct {
    struct sockaddr          *sockaddr;
    socklen_t                 socklen;
    bnx_str_t                 name;
} bnx_addr_t;


typedef struct {
    bnx_str_t                 url;
    bnx_str_t                 host;
    bnx_str_t                 port_text;
    bnx_str_t                 url;

    in_port_t                 port;
    in_port_t                 default_port;
    in_port_t                 last_port;
    int                       family;

    unsigned                  listen:1;
    unsigned                  uri_part:1;
    unsigned                  no_resolve:1;

    unsigned                  no_port:1;
    unsigned                  wildcard:1;

    socklen_t                 socklen;
    bnx_sockaddr_t            sockaddr;

    bnx_addr_t               *addrs;
    bnx_uint_t                naddrs;

    char                     *err;
} bnx_url_t;


in_addr_t bnx_inet_addr(u_char *text, size_t len);
#if (BNX_HAVE_INET6)
bnx_int_t bnx_inet6_addr(u_char *p, size_t len, u_char *addr);
size_t bnx_inet6_ntop(u_char *p, u_char *text, size_t len);
#endif
size_t bnx_sock_ntop(struct sockaddr *sa, socklen_t socklen, u_char *text,
    size_t len, bnx_uint_t port);
size_t bnx_inet_ntop(int family, void *addr, u_char *text, size_t len);
bnx_int_t bnx_ptocidr(bnx_str_t *text, bnx_cidr_t *cidr);
bnx_int_t bnx_cidr_match(struct sockaddr *sa, bnx_array_t *cidrs);
bnx_int_t bnx_parse_addr(bnx_pool_t *pool, bnx_addr_t *addr, u_char *text,
    size_t len);
bnx_int_t bnx_parse_addr_port(bnx_pool_t *pool, bnx_addr_t *addr,
    u_char *text, size_t len);
bnx_int_t bnx_parse_url(bnx_pool_t *pool, bnx_url_t *u);
bnx_int_t bnx_inet_resolve_host(bnx_pool_t *pool, bnx_url_t *u);
bnx_int_t bnx_cmp_sockaddr(struct sockaddr *sa1, socklen_t slen1,
    struct sockaddr *sa2, socklen_t slen2, bnx_uint_t cmp_port);
in_port_t bnx_inet_get_port(struct sockaddr *sa);
void bnx_inet_set_port(struct sockaddr *sa, in_port_t port);
bnx_uint_t bnx_inet_wildcard(struct sockaddr *sa);


#endif /* _BNX_INET_H_INCLUDED_ */