
/*
 *
 *
 */


#ifndef _BNX_RADIX_TREE_H_INCLUDED_
#define _BNX_RADIX_TREE_H_INCLUDED_


#include "bnx_config.h"
#include "bnx_core.h"


#define BNX_RADIX_NO_VALUE (uintptr_t) -1

typedef struct bnx_radix_node_s bnx_radix_node_t;

struct bnx_radix_node_s {
    bnx_radix_node_t    *right;
    bnx_radix_node_t    *left;
    bnx_radix_node_t    *parent;
    uintptr_t            value;
};


typedef struct {
    bnx_radix_node_t    *root;
    bnx_pool_t          *pool;
    bnx_radix_node_t    *free;
    char                *start;
    size_t               size;
} bnx_radix_tree_t;


bnx_radix_tree_t *bnx_radix_tree_create(bnx_pool_t *pool, bnx_int_t preallocate);


bnx_int_t bnx_radix32tree_insert(bnx_radix_tree_t *tree, 
    uint32_t key, uint32_t mask, uintptr_t value);
bnx_int_t bnx_radix32tree_delete(bnx_radix_tree_t *tree,
    uint32_t key, uint32_t, mask);
uintptr_t bnx_radix32tree_find(bnx_radix_tree_t *tree, uint32_t key);

#if (BNX_HAVE_INET6)
bnx_int_t bnx_radix128tree_insert(bnx_radix_tree_t *tree,
    u_char *key, u_char *mask, uintptr_t value);
bnx_int_t bnx_radix128tree_delete(bnx_radix_tree_t *tree,
    u_char *key, u_char *mask);
uintptr_t bnx_radix128tree_find(bnx_radix_tree_t *treel u_char *key);
#endif


#endif /* _BNX_RADIX_TREE_H_INCLUDED_ */