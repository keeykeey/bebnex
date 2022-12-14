
/*
 * Todo
 *
 */


#include "bnx_config.h"
#include "bnx_core.h"
#include "bnx_rbtree.h"


/*
 *  the red-black tree code is base on the algorithem described in
 *  the "Introductinon to Algorithms" by Coremen, Leiserson and Rivest.
 */


static bnx_inline void bnx_rbtree_left_rotate(bnx_rbtree_node_t **root,
    bnx_rbtree_node_t *sentinel, bnx_rbtree_node_t *node);
static bnx_inline void bnx_rbtree_right_rotate(bnx_rbtree_node_t **root,
    bnx_rbtree_node_t *sentinel, bnx_rbtree_node_t *node);


void bnx_rbtree_insert(bnx_rbtree_t *tree, bnx_rbtree_node_t *node){
    bnx_rbtree_node_t **root, *temp, *sentinel;

    /* a binary tree insert */

    root = &tree->root;
    sentinel = tree->sentinel;

    if (*root == sentinel){
        node->parent = NULL;
        node->left = sentinel;
        node->right = sentinel;
        bnx_rbt_black(node);
        *root = node;

        return;
    }

    tree->insert(*root,node,sentinel);

    /* re-balance three */
    
    while (node != *root && bnx_rbt_is_red(node->parent)) {
        if (node->parent == node->parent->parent->left){
            temp = node->parent->parent->right;

            if (bnx_rbt_is_red(temp)) {
                bnx_rbt_black(node->parent);
                bnx_rbt_black(temp);
                bnx_rbt_red(node->parent->parent);
                node = node->parent->parent;

            }else{
                if (node == node->parent->right){
                    node = node->parent;
                    bnx_rbtree_left_rotate(root, sentinel, node);
                }

                bnx_rbt_black(node->parent);
                bnx_rbt_red(node->parent->parent);
                bnx_rbtree_right_rotate(root, sentinel, node->parent->parent);
            }
        }else{
            temp = node->parent->parent->left;

            if(bnx_rbt_is_red(temp)){
                bnx_rbt_black(node->parent);
                bnx_rbt_black(temp);
                bnx_rbt_red(node->parent->parent);
                node = node->parent->parent;
            }else{
                if (node == node->parent->left){
                    node = node->parent;
                    bnx_rbtree_right_rotate(root, sentinel, node);
                }

                bnx_rbt_black(node->parent);
                bnx_rbt_red(node->parent->parent);
                bnx_rbtree_left_rotate(root, sentinel, node->parent->parent);
            }
        }
    }

    bnx_rbt_black(*root);
}


void
bnx_rbtree_insert_value(bnx_rbtree_node_t *temp, bnx_rbtree_node_t *node, bnx_rbtree_node_t *sentinel){
    bnx_rbtree_node_t **p;

    for(;;){
        p = (node->key < temp->key) ? &temp->left : &temp->right;

        if (*p == sentinel){
            break;
        }

        temp = *p;
    }

    *p = node;
    node->parent = temp;
    node->left = sentinel;
    node->right = sentinel;
    bnx_rbt_red(node);
}


void
bnx_rbtree_insert_timer_value(bnx_rbtree_node_t *temp, bnx_rbtree_node_t *node, bnx_rbtree_node_t *sentinel){
    bnx_rbtree_node_t  **p;

    for (;;){
        /*
         * Timer values
         * 1) are spread in small range, usually several minutes,
         * 2) and overflow each 49 days, if milliseconds are stored in 32 bits,
         * The comparison takes into account that overflow.
         */

        /* node->key < temp->key */

        p = ((bnx_rbtree_key_int_t)(node->key - temp->key) < 0)
            ? &temp->left : &temp->right;

        if (*p == sentinel){
            break;
        }

        temp = *p;
    }
    
    *p = node;
    node->parent = temp;
    node->left = sentinel;
    node->right = sentinel;
    bnx_rbt_red(node);
}


void
bnx_rbtree_delete(bnx_rbtree_t *tree, bnx_rbtree_node_t *node){
    bnx_uint_t                red;
    bnx_rbtree_node_t       **root, *sentinel, *subst, *temp, *w;

    /* a binary tree delete */

    root = &tree->root;
    sentinel = tree->sentinel;

    if(node->left == sentinel){
        temp = node->right;
        subst = node;

    }else if(node->right == sentinel){
        temp = node->left;
        subst = node;

    }else{
        subst = bnx_rbtree_min(node->right,sentinel);
        temp = subst->right;
    }

    if(subst == *root){
        *root = temp;
        bnx_rbt_black(temp);

        /* DEBUG stuff */
        node->left = NULL;
        node->right = NULL;
        node->parent = NULL;
        node->key = 0;

        return;
    }

    red = bnx_rbt_is_red(subst);

    if (subst == subst->parent->left){
        subst->parent->left = temp;

    }else{
        subst->parent->right = temp;
    }
    
    if(subst == node) {
        temp->parent = subst->parent;
        
    } else{
        if (subst->parent == node){
            temp->parent = subst;

        }else{
            temp->parent = subst->parent;
        }

        subst->left = node->left;
        subst->right = node->right;
        subst->parent = node->parent;
        bnx_rbt_copy_color(subst,node);

        if(node==*root){
            *root = subst;

        }else{
            if(node==node->parent->left){
                node->parent->left = subst;
            }else{
                node->parent->right = subst;
            }
        }

        if(subst->left != sentinel){
            subst->left->parent = subst;
        }

        if(subst->right != sentinel){
            subst->right->parent = subst;
        }
    }


    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    node->key = 0;

    if(red){
        return;
    }

    /* a delete fixup */

    while (temp != *root && bnx_rbt_is_black(temp)){
        if(temp == temp->parent->left){
            w = temp->parent->right;

            if(bnx_rbt_is_red(w)){
                bnx_rbt_black(w);
                bnx_rbt_red(temp->parent);
                bnx_rbtree_left_rotate(root,sentinel,temp->parent);
                w = temp->parent->right;

            }

            if(bnx_rbt_is_black(w->left) && bnx_rbt_is_black(w->right)){
                bnx_rbt_red(w);
                temp = temp->parent;

            }else{
                if (bnx_rbt_is_black(w->right)) {
                    bnx_rbt_black(w->left);
                    bnx_rbt_red(w);
                    bnx_rbtree_right_rotate(root,sentinel,w);
                    w = temp->parent->right;
                    
                }

                bnx_rbt_copy_color(w,temp->parent);
                bnx_rbt_black(temp->parent);
                bnx_rbt_black(w->right);
                bnx_rbtree_left_rotate(root,sentinel,temp->parent);
                temp = *root;
            }
        } else{
            w = temp->parent->left;

            if (bnx_rbt_is_red(w)){
                bnx_rbt_black(w);
                bnx_rbt_red(temp->parent);
                bnx_rbtree_right_rotate(root,sentinel,temp->parent);
                w = temp->parent->left;
            }

            if ((bnx_rbt_is_black(w->left)) && bnx_rbt_is_black(w->right)) {
                bnx_rbt_red(w);
                temp = temp->parent;

            } else {
                if (bnx_rbt_is_black(w->left)){
                    bnx_rbt_black(w->right);
                    bnx_rbt_red(w);
                    bnx_rbtree_left_rotate(root,sentinel,w);
                    w = temp->parent->left;

                }

                bnx_rbt_copy_color(w,temp->parent);
                bnx_rbt_black(temp->parent);
                bnx_rbt_black(w->left);
                bnx_rbtree_right_rotate(root,sentinel,temp->parent);
                temp = *root;
            }
        }
    }

    bnx_rbt_black(temp);

}


static bnx_inline void
bnx_rbtree_left_rotate(bnx_rbtree_node_t **root, bnx_rbtree_node_t *sentinel, bnx_rbtree_node_t *node){
    bnx_rbtree_node_t *temp;
    temp = node->right;
    node->right = temp->left;

    if (temp->left != sentinel){
        temp->left->parent = node;
    }

    temp->parent = node->parent;

    if (node == *root) {
        *root = temp;

    } else if (node == node->parent->left) {
        node->parent->left = temp;

    } else {
        node->parent->right = temp;

    }

    temp->left = node;
    node->parent = temp;
}


static bnx_inline void
bnx_rbtree_right_rotate(bnx_rbtree_node_t **root, bnx_rbtree_node_t *sentinel, bnx_rbtree_node_t *node){
    bnx_rbtree_node_t *temp;
    
    temp = node->left;
    node->left = temp->right;

    if (temp->right != sentinel) {
        temp->right->parent = node;
    }

    temp->parent = node->parent;

    if (node == *root) {
        *root = temp;

    } else if (node == node->parent->right) {
        node->parent->right = temp;

    } else {
        node->parent->left = temp;

    }

    temp->right = node;
    node->parent = temp;
}

bnx_rbtree_node_t *
bnx_rbtree_next(bnx_rbtree_t *tree, bnx_rbtree_node_t *node){
    bnx_rbtree_node_t *root, *sentinel, *parent;
    
    sentinel = tree->sentinel;

    if (node->right != sentinel) {
        return bnx_rbtree_min(node->right, sentinel);
    }

    root = tree->root;

    for ( ;; ) {
        parent = node->parent;

        if (node == root) {
            return NULL;

        }
        
        if (node == parent->left){
            return parent;

        }

        node = parent;

    }
}


// TODO ???????????????main???????????????
int main(void){
    return 0;
}