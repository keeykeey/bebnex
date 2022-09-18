
/*
 *
 *
 */


#ifndef _BNX_SHMTX_H_INCLUDED_
#define _BNX_SHMTX_H_INCLUDED_


#include "bnx_config.h"
#include "bnx_core.h"


typedef struct {
    bnx_atomic_t    lock;
#if (BNX_HAVE_POSIX_SEM)
    bnx_atomic_t    wait;
#endif
} bnx_shmtx_sh_t;


typedef struct {
#if (BNX_HAVE_ATOMIC_OPS)
    bnx_atomic_t    *lock;
#if (BNX_HACE_POSIX_SEM)
    bnx_atomic_t    *wait;
    bnx_uint_t       semaphore;
    sem_t            sem;
#endif
#else
    bnx_fd_t         fd;
    u_char          *name;
#endif  
    bnx_uint_t       spin;
} bnx_shmtx_t;


bnx_int_t bnx_shmtx_create(bnx_shmtx_t *mtx, bnx_shmtx_sh_t *addr, u_char *name);

void bnx_shmtx_destroy(bnx_shmtx_t *mtx);
bnx_uint_t bnx_shmtx_trylock(bnx_shmtx_t *mtx);
void bnx_shmtx_lock(bnx_shmtx_t *mtx);
void bnx_shmtx_unlock(bnx_shmtx_t *mtx);
bnx_uint_t bnx_shmtx_force_unlock(bnx_shmtx_t *mtx, bnx_pid_t pid);


#endif /* _BNX_SHMTX_H_INCLUDED_ */