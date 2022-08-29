
/*
 * Todo Copyright
 *
 */


#ifndef _BNX_RBTREE_H_INCLUDED
#define _BNX_RBTREE_H_INCLUDED


#include "bnx_config.h"
#include "bnx_core.h"


typedef bnx_uint_t bnx_rbtree_key_t;
typedef bnx_int_t bnx_rbtree_key_int_t;


typedef struct bnx_rbtree_node_s bnx_rbtree_node_t;

struct bnx_rbtree_node_s {
    bnx_rbtree_key_t key;
    bnx_rbtree_node_t *left;
    bnx_rbtree_node_t *right;
    bnx_rbtree_node_t *parent;
    u_char color;
    u_char data;
};


typedef struct bnx_rbtree_s bnx_rbtree_t;

typedef void (*bnx_rbtree_insert_pt) (bnx_rbtree_node_t *root,
    bnx_rbtree_node_t *node,bnx_rbtree_node_t *sentinel);

struct bnx_rbtree_s {
    bnx_rbtree_node_t     *root;
    bnx_rbtree_node_t     *sentinel;
    bnx_rbtree_insert_pt   insert;
};


#define bnx_rbtree_init(tree,s,i) \
    bnx_rbtree_sentinel_init(s);  \
    (tree)->root = s;             \
    (tree)->sentinel = s;         \
    (tree)->insert = i            \

#define bnx_rbtree_data(node, type, link) \
    (type*) ((u_char *) (node) - offsetof(type,link))


void bnx_rbtree_insert(bnx_rbtree_t *tree, bnx_rbtree_node_t *node);
void bnx_rbtree_delete(bnx_rbtree_t *tree, bnx_rbtree_node_t *node);
void bnx_rbtree_insert_value(bnx_rbtree_node_t *root, bnx_rbtree_node_t *node,
    bnx_rbtree_node_t *sentinel);
void bnx_rbtree_insert_timer_value(bnx_rbtree_node_t *root,
    bnx_rbtree_node_t *node, bnx_rbtree_node_t *sentinel);
bnx_rbtree_node_t *bnx_rbtree_next(bnx_rbtree_t *tree,
    bnx_rbtree_node_t *node);


#define bnx_rbt_red(node)          ((node)->color = 1)
#define bnx_rbt_black(node)        ((node)->color = 0)
#define bnx_rbt_is_red(node)       ((node)->color)
#define bnx_rbt_is_black(node)     (!bnx_rbt_is_red(node))
#define bnx_rbt_copy_color(n1,n2)  (n1->color = n2->color)


/* a sentinel must be black */

#define bnx_rbtree_sentinel_init(node)  bnx_rbt_black(node)


/*
static bnx_inline bnx_rbtree_node_t *
bnx_rbtree_min(bnx_rbtree_node_t *node, bnx_rbtree_node_t *sentinel){
    while (node->left != sentinel){
        node = node->left;
    }

    return node;
}
*/

#endif /* _BNX_RBTREE_H_INCLUDED_ */