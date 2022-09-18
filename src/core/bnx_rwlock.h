
/*
 *
 *
 */


#ifndef _BNX_RWLOCK_H_INCLUDED_
#define _BNX_RWLOCK_H_INCLUDED_


#include "bnx_config.h"
#include "bnx_core.h"


void bnx_rwlock_wlock(bnx_atomic_t *lock);
void bnx_rwlock_rlock(bnx_atomic_t *lock);
void bnx_rwlock_unlock(bnx_atomic_t *lock);
void bnx_rwlock_downgrade(bnx_atomic_t *lock);


#endif /* _BNX_RWLOCK_H_INCLUDED_ */