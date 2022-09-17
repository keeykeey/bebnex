
/*
 *
 *
 *
 */


#ifndef _BNX_MODULE_H_INCLUDED_
#define _BNX_MODULE_H_INCLUDED_


#include "bnx_config.h"
#include "bnx_core.h"
#include "bebnex.h"


#define BNX_MODULE_UNSET_INDEX (bnx_uint_t) -1


#define BNX_MODULE_SIGNATURE_0            \
    bnx_value(BNX_PTR_SIZE) ","           \
    bnx_value(BNX_SIG_ATOMIC_T_SIZE) ","  \
    bnx_value(BNX_TIME_T_SIZE) ","

#if (BNX_HAVE_KQUEUE)
#define BNX_MODULE_SIGNATURE_1  "1"
#else
#define BNX_MODULE_SIGNATURE_1  "0"
#endif

#if (BNX_HAVE_IOCP)
#define BNX_MODULE_SIGNATURE_2  "1"
#else
#define BNX_MODULE_SIGNATURE_2  "0"
#endif

#if (BNX_HAVE_FILE_AIO || BNX_COMPAT)
#define BNX_MODULE_SIGNATURE_3  "1"
#else   
#define BNX_MODULE_SIGNATURE_3  "0"
#endif

#if (BNX_HAVE_SENDFILE_NODISKIO || BNX_COMPAT)
#define BNX_MODULE_SIGNATURE_4  "1"
#else 
#define BNX_MODULE_SIGNATURE_4  "0"
#endif

#if (BNX_HAVE_EVENTFD)
#define BNX_MODULE_SIGNATURE_S "1"
#else
#define BNX_MODULE_SIGNATURE_5 "0"
#endif

#if (BNX_HAVE_EPOLL)
#define BNX_MODULE_SIGNATURE_6  "1"
#else 
#define BNX_MODULE_SIGNATURE_6  "0"
#endif

#if (BNX_HAVE_KEEPALIVE_TUNABLE)
#define BNX_MODULE_SIGNATURE_7  "1"
#else
#define BNX_MODULE_SIGNATURE_7  "0"
#endif

#if (BNX_HAVE_INET6)
#define BNX_MODULE_SIGNATURE_8 "1"
#else 
#define BNX_MODULE_SIGNATURE_8 "0"
#endif

#define BNX_MODULE_SIGNATURE_9 "1"
#define BNX_MODULE_SIGNATURE_10 "1"

#if (BNX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)
#define BNX_MODULE_SIGNATURE_11 "1"
#else
#define BNX_MODULE_SIGNATURE_11 "0"
#endif

#define BNX_MODULE_SIGNATURE_12  "1"

#if (BNX_HAVE_SETFIB)
#define BNX_MODULE_SIGNATURE_13  "1"
#else 
#define BNX_MODULE_SIGNATURE_13 "0"
#endif

#if (BNX_HAVE_TCP_FASTOPEN)
#define BNX_MODULE_SIGNATURE_14  "1"
#else
#define BNX_MODULE_SIGNATURE_14  "0"
#endif

#if (BNX_HAVE_UNIX_DOMAIN)
#define BNX_MODULE_SIGNATURE_15  "1"
#else
#define BNX_MODULE_SIGNATURE_15  "0"
#endif

#if (BNX_HAVE_VARIADIC_MACROS)
#define BNX_MODULE_SIGNATURE_16  "1"
#else
#define BNX_MODULE_SIGNATURE_16  "0"
#endif

#define BNX_MODULE_SIGNATURE_17  "0"
#define BNX_MODULE_SIGNATURE_18  "0"

#if (BNX_HAVE_OPENAT)
#define BNX_MODULE_SIGNATURE_19  "1"
#else
#define BNX_MODULE_SIGNATURE_19  "0"
#endif

#if (BNX_HAVE_ATOMIC_OPS)
#define BNX_MODULE_SIGNATURE_20  "1"
#else 
#define BNX_MODULE_SIGNATURE_20  "0"
#endif

#if (BNX_HAVE_POSIX_SEM)
#define BNX_MODULE_SIGNATURE_21  "1"
#else
#define BNX_MODULE_SIGNATURE_21  "0"
#endif

#if (BNX_THREADS || BNX_COMPAT)
#define BNX_MODULE_SIGNATURE_22  "1"
#else 
#define BNX_MODULE_SIGNATURE_22  "0"
#endif

#if (BNX_PCRE)
#define BNX_MODULE_SIGNATURE_23  "1"
#else
#define BNX_MODULE_SIGNATURE_23  "0"
#endif

#if (BNX_HTTP_SSL || BNX_COMPAT)
#define BNX_MODULE_SIGNATURE_24  "1"
#else 
#define BNX_MODULE_SIGNATURE_24  "0"
#endif

#define BNX_MODULE_SIGNATURE_25  "1"

#if (BNX_HTTP_GZIP)
#define BNX_MODULE_SIGNATURE_26  "1"
#else
#define BNX_MODULE_SIGNATURE_26  "0"
#endif

#define BNX_MODULE_SIGNATURE_27  "1"

#if (BNX_HTTP_XFORWARDED_FOR)
#define BNX_MODULE_SIGNATURE_28  "1"
#else 
#define BNX_MODULE_SIGNATURE_28  "0"
#endif

#if (BNX_HTTP_REALIP)
#define BNX_MODULE_SIGNATURE_29  "1"
#else
#define BNX_MODULE_SIGNATURE_29  "0"
#endif

#if (BNX_HTTP_HEADERS)
#define BNX_MODULE_SIGNATURE_30  "1"
#else
#define BNX_MODULE_SIGNATURE_30  "0"
#endif

#if (BNX_HTTP_DAV)
#define BNX_MODULE_SIGNATURE_31  "1"
#else
#define BNX_MODULE_SIGNATURE_31  "0"
#endif

#if (BNX_HTTP_CACHE)
#define BNX_MODULE_SIGNATURE_32  "1"
#else
#define BNX_MODULE_SIGNATURE_32  "0"
#endif

#if (BNX_HTTP_UPSTREAM_ZONE)
#define BNX_MODULE_SIGNATURE_33  "1"
#else
#define BNX_MOSULE_SIGNATURE_33  "0"
#endif

#if (BNX_COMPAT)
#define BNX_MODULE_SIGNATURE_34  "1"
#else
#define BNX_MODULE_SIGNATURE_34  "0"
#endif

#define BNX_MODULE_SIGNATURE  \
    BNX_MODULE_SIGNATURE_0 BNX_MODULE_SIGNATURE_1 BNX_MODULE_SIGNATURE_2 \
    BNX_MODULE_SIGNATURE_3 BNX_MODULE_SIGNATURE_4 BNX_MODULE_SIGNATURE_5 \
    BNX_MODULE_SIGNATURE_6 BNX_MODULE_SIGNATURE_7 BNX_MODULE_SIGNATURE_8 \
    BNX_MODULE_SIGNATURE_9 BNX_MODULE_SIGNATURE_10 BNX_MODULE_SIGNATURE_11 \
    BNX_MODULE_SIGNATURE_12 BNX_MODULE_SIGNATURE_13 BNX_MODULE_SIGNATURE_14 \
    BNX_MODULE_SIGNATURE_15 BNX_MODULE_SIGNATURE_16 BNX_MODULE_SIGNATURE_17 \
    BNX_MODULE_SIGNATURE_18 BNX_MODULE_SIGNATURE_19 BNX_MODULE_SIGNATURE_20 \
    BNX_MODULE_SIGNATURE_21 BNX_MODULE_SIGNATURE_22 BNX_MODULE_SIGNATURE_23 \
    BNX_MODULE_SIGNATURE_24 BNX_MODULE_SIGNATURE_25 BNX_MODULE_SIGNATURE_26 \
    BNX_MODULE_SIGNATURE_27 BNX_MODULE_SIGNATURE_28 BNX_MODULE_SIGNATURE_29 \
    BNX_MODULE_SIGNATURE_30 BNX_MODULE_SIGNATURE_31 BNX_MODULE_SIGNATURE_32 \
    BNX_MODULE_SIGNATURE_33 BNX_MODULE_SIGNATURE_34 


#define BNX_MODULE_V1    \
    BNX_MODULE_UNSET_INDEX, BNX_MODULE_UNSET_INDEX,    \
    NULL, 0, 0, bebnex_version, BNX_MODULE_SIGNATURE

#define bnx_module_v1_padding 0, 0, 0, 0, 0, 0, 0, 0


struct bnx_module_s {
    bnx_uint_t       stx_index;
    bnx_uint_t       index;

    char            *name;

    bnx_uint_t       spare0;
    bnx_uint_t       spare1;
    
    bnx_uint_t       version;
    const char      *signature;

    void            *ctx;
    bnx_command_t    commands;
    bnx_uint_t       type;

    bnx_int_t      (*init_master)(bnx_log_t *log);
    
    bnx_int_t      (*init_module)(bnx_cycle_t *cycle);

    bnx_int_t      (*init_process)(bnx_cycle_t *cycle);
    bnx_int_t      (*init_thread)(bnx_cycle_t *cycle);
    void           (*exit_thread)(bnx_cycle_t *cycle);
    void           (*exit_process)(bnx_cycle_t *cycle);

    void           (*exit_master)(bnx_cycle_t *cycle);

    uintptr_t        spare_hook0;
    uintptr_t        spare_hook1;
    uintptr_t        spare_hook2;
    uintptr_t        spare_hook3;
    uintptr_t        spare_hook4;
    uintptr_t        spare_hook5;
    uintptr_t        spare_hook6;
    uintptr_t        spare_hook7;
};


typedef struct {
    bnx_str_t        name;
    void          *(*create_conf)(bnx_cycle_t *cycle);
    char          *(*init_conf)(bnx_cycle_t, void *conf);
} bnx_core_module_t;


bnx_int_t bnx_preinit_modules(void);
bnx_int_t bnx_cycle_modules(bnx_cycle_t *cycle);
bnx_int_t bnx_init_modules(bnx_cycle_t *cycle);
bnx_int_t bnx_count_modules(bnx_cycle_t *cycle, bnx_uint_t type);


bnx_int_t bnx_add_module(bnx_conf_t *cf, bnx_str_t *file, bnx_module_t *module, char **order);



extern bnx_module_t *bnx_modules[];
extern bnx_uint_t   *bnx_max_module;

extern char         *bnx_module_names[]


#endif /* _BNX_MODULE_H_INCLUDED_ */