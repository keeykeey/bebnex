
/*
 *
 *
 */


#ifndef _BNX_SHA1_H_INCLUDED_
#define _BNX_SHA1_H_INCLUDED_


#include "bnx_config.h"
#include "bnx_core.h"


typedef struct {
    uint64_t bytes;
    uint32_t a,b,c,d,e,f;
    u_char   buffer[64];
} bnx_sha1_t;


void bnx_sha1_init(bnx_sha1_t *ctx);
void bnx_sha1_update(bnx_sha1_t *ctx, const void *data, size_t size);
void bnx_sha1_final(u_char result[20], bnx_sha1_t *ctx);


#endif /* _BNX_SHA1_H_INCLUDED_ */