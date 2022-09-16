
/*
 *
 *
 */


#ifndef _BNX_MD5_H_INCLUDED_
#define _BNX_MD5_H_INCLUDED_


#include "bnx_conf.h"
#include "bnx_core.h"


typedef struct {
    uint64_t     bytes;
    uint32_t     a, b, c, d;
    u_char       buffer[64];
} bnx_md5_t;


void bnx_md5_init(bnx_md5_t *ctx);
void bnx_md5_update(bnx_md5_t *ctx, const void *data, size_t size);
void bnx_md5_final(u_char result[16], bnx_md5_t *ctx);


#endif /* _BNX_MD5_H_INCLUDED_ */