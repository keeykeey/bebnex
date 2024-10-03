#ifndef BNX_INET_H
#define BNX_INET_H

#define BNX_INET_ADDR_STRLEN    (sizeof("255.255.255.255") - 1)
#define BNX_INET6_ADDR_STRLEN    \
    (sizeof("ffff:ffff:ffff:ffff:ffff:ffff:255.255.255.255") - 1)
#define BNX_SOCKADDR_STRLEN    (BNX_INET_ADDR_STRLEN + sizeof(":65535") - 1)

#if !defined(IPV6_V6ONLY)
#define IPV6_V6ONLY 27
#endif

#endif /** BNX_INET_H */