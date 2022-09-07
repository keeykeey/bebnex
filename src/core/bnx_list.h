
/*
 *
 *
 */


 #ifndef _BNX_LIST_H_INCLUDED_
 #define _BNX_LIST_H_INCLUDED_


 #include "bnx_config.h"
 #include "bnx_core.h"


typedef struct bnx_list_part_s bnx_list_part_t;

struct bnx_list_part_s {
    void                *elts;
    bnx_unit_t           nelts;
    bnx_list_part_t     *next;
};


typedef struct {
    bnx_list_part_t    *last;
    bnx_list_part_t     part;
    size_t              size;
    bnx_uint_t          nalloc;
    bnx_pool_t         *pool;
}bnx_list_t;


bnx_list_t  *bnx_list_create(bnx_pool_t *pool, bnx_uint_t n, size_t size);

static bnx_inline bnx_int_t
bnx_list_int(bnx_list_t *list, bnx_pool, bnx_uint_t n, size_t size){
    list->part.elts = bnx_palloc(pool, n * size);
    if (list->part.elts == NULL){
        return BNX_ERROR;
    }

    list->part.nelts = 0;
    list->part.next = NULL;
    list->last = &list->part;
    list->size = size;
    list->nalloc = n;
    list->pool = pool;

    return BNX_OK;
}


/* 
 *The iteration through the list;
   
   part = &list.part;
   data = part->elts;

   for (i=0;;i++) {

       if (i >= part->nelts ){
           if (part->next == NULL){
               break;
           }

           part = part->next;
           data = part->elts;
           i = 0;
       }
       ...  data[i] ...
   }

 */


void *bnx_list_push(bnx_list_t *list);


#endif /*  _BNX_LIST_H_INCLUDED_ */








