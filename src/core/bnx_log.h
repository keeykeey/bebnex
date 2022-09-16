
/*
 * Todo
 *
 */


#ifndef _BNX_LOG_H_INCLUDED_
#define _BNX_LOG_H_INCLUDED_


#include "bnx_config.h"
#include "bnx_core.h"


#define BNX_LOG_STDERR            0
#define BNX_LOG_EMERG             1
#define BNX_LOG_ALERT             2
#define BNX_LOG_CRIT              3
#define BNX_LOG_ERR               4
#define BNX_LOG_WARN              5
#define BNX_LOG_NOTICE            6
#define BNX_LOG_INFO              7
#define BNX_LOG_DEBUG             8

#define BNX_LOG_DEBUG_CORE        0x010
#define BNX_LOG_DEBUG_ALLOC       0x020
#define BNX_LOG_DEBUG_MUTEX       0x040
#define BNX_LOG_DEBUG_HTTP        0x100
#define BNX_LOG_DEBUT_MAIL        0x200
#define BNX_LOG_DEBUG_STREAM      0x400

/*
 *  do not forget to update debug_levels[] in src/core/bnx_log.c
 *  after adding a new debug level
 */


#define BNX_LOG_DEBUG_FIRST            BNX_LOG_DEBUG_CORE
#define BNX_LOG_DEBUG_LAST             BNX_LOG_DEBUG_STREAM
#define BNX_LOG_DEBUG_CONNECTION       0x80000000
#define BNX_LOG_DEBUG_ALL              0x7ffffff0


typedef u_char  *(*bnx_log_handler_pt) (bnx_log_t *log, u_char *buf, size_t len);
typedef void (*bnx_log_writer_pt)(bnx_log_t *log, bnx_uint_t level, u_char *buf, size_t len);


struct bnx_log_s {
    bnx_uint_t                log_level;
    bnx_open_file_t          *file;

    bnx_atomic_uint_t         connection;

    time_t                    disk_full_time;

    bnx_log_handler_pt        handler;
    void                     *data;

    bnx_log_writer_pt         writer;
    void                     *wdata;
    
    /*
     * we declare "action" as "char *" because the actions are usually
     * the static strings and in the "u_char *" case we have to override
     * their typed all the time
     */
};


/************************************************/

#if (BNX_HAVE_C99_VARIADIC_MACROS)

#define BNX_HAVE_VARIADIC_MACROS  1

#define bnx_log_error(level,log, ...) \
    if ((log)->log_level >= level) bnx_log_error_core(level, log, __VA_ARGS__)

void bnx_log_error_core(bnx_uint_t level, bnx_log_t *log, bnx_err_t err, const char *fmt, ...);

#define bnx_log_debug(level, log, ...)
    if ((log)->log_level & level)
        bnx_log_error_core(BNX_LOG_DEBUG, log, __VA_ARGS__)

/************************************************/

#elif (BNX_HAVE_GCC_VARIADIC_MACROS)

#define BNX_HAVE_VARIADIC_MACROS    1

#define bnx_log_error(level, log, args...)
    if ((log)->log_level >= level) bnx_log_error_core(level, log, args)

void bnx_log_error_core(bnx_uint_t level, bnx_log_t *log, bnx_err_t err, const char *fmt, ...);

#define bnx_log_debug(level, log, args...)
    if ((log)->log_level & level)
        bnx_log_error_core(BNX_LOG_DEBUG, log, args)

/*************************************************/

#else /* no variadic macros */

#define BNX_HAVE_VARIADIC_MACROS  0

void bnx_cdecl bnx_log_error(bnx_uint_t level, bnx_log_t *log, bnx_err_t err, const char *fmt, ...);
void bnx_log_error_core(bnx_uint_t level, bnx_log_t *log, bnx_err_t err, const char *fmt, va_list args);
void bnx_cdecl bnx_log_debug_core(bnx_log_t *log, bnx_err_t err, const char *fmt, ...);

#endif /* variadic macros */

/*************************************************/

#if (BNX_DEBUG)

#if (BNX_HAVE_VARIADIC_MACROS)

#define bnx_log_debug0(level, log, err, fmt) \
        bnx_log_debug(level, log, err, fmt)

#define bnx_log_debug1(level, log, err, fmt, arg1)  \
        bnx_log_debug(level, log, err, fmt, arg1)

#define bnx_log_debug2(level, log, err, fmt, arg1, arg2)  \
        bnx_log_debug(level, log, err, fmt, arg1, arg2)

#define bnx_log_debug3(level, log, err, fmt, arg1, arg2, arg3)  \
        bnx_log_debug(level, log, err, fmt, arg1, arg2, arg3)

#define bnx_log_debug4(level, log, err, fmt, arg1, arg2, arg3, arg4)  \
        bnx_log_debug(level, log, err, fmt, arg1, arg2, arg3, arg4)

#define bnx_log_debug5(level, log, err, fmt, arg1, arg2, arg3, arg4, arg5)  \
        bnx_log_debug(level, log, err, fmt, arg1, arg2, arg3, arg4, arg5)

#define bnx_log_debug6(level, log, err, fmt,  \
                       arg1, arg2, arg3, arg4, arg5, arg6)  \
        bnx_log_debug(level, log, err, fmt, \
                      arg1, arg2, arg3, arg4, arg5, arg6)

#define bnx_log_debug7(level, log, err, fmt,  \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7)  \
        bnx_log_debug(level, log, err, fmt,   \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7)

#define bnx_log_debug8(level, log, err, fmt,  \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)  \
        bnx_log_debug(level, log, err, fmt,   \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)

#else  /* no variadic macros */

#define bnx_log_debug0(level, log, err, fmt)    \
    if ((log)->log_level & level)               \
        bnx_log_debug_core(log, err, fmt)

#define bnx_log_debug1(level, log, err, fmt, arg1)  \
    if ((log)->log_level & level)                   \
        bnx_log_debug_core(log, err, fmt, arg1)

#define bnx_log_debug2(level, log, err, fmt, arg1, arg2)  \
    if ((log)->log_level & level)                         \
        bnx_log_debug_core(log, err, fmt, arg1, arg2) 

#define bnx_log_debug3(level, log, err, fmt, arg1, arg2, arg3)    \
    if ((log)->log_level & level)                                 \
        bnx_log_debug_core(log, err, fmt, arg1, arg2, arg3)

#define bnx_log_debug4(level, log, err, fmt, arg1, arg2, arg3, arg4)    \
    if ((log)->log_level & level)                                       \
        bnx_log_debug_core(log, err, fmt, arg1, arg2, arg3, arg4)

#define bnx_log_debug5(level, log, err, fmt, arg1, arg2, arg3, arg4, arg5)    \
    if ((log)->log_level & level)                                             \
        bnx_log_debug_core(log, err, fmt, arg1, arg2, arg3, arg4, arg5)

#define bnx_log_debug6(level, log, err, fmt                                   \
                       arg1, arg2, arg3, arg4, arg5, arg6)                    \
    if ((log)->log_level & level)                                             \
        bnx_log_debug_core(log, err, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define bnx_log_debug7(level, log, err, fmt,                                  \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7)              \
    if((log)->log_level & level)                                              \
        bnx_log_debug_core(log, err, fmt,                                     \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7)

#define bnx_log_debug8(level, log, err, fmt,                                  \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)        \
    if((log)->log_level & level)                                              \
        bnx_log_debug_core(log, err, fmt,                                     \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)

#endif

#else /* !BNX)DEBUG */

#define bnx_log_debug0(level, log, err, fmt)
#define bnx_log_debug1(level, log, err, fmt, arg1)
#define bnx_log_debug2(level, log, err, fmt, arg1, arg2)
#define bnx_log_debug3(level, log, err, fmt, arg1, arg2, arg3)
#define bnx_log_debug4(level, log, err, fmt, arg1, arg2, arg3, arg4)
#define bnx_log_debug5(level, log, err, fmt, arg1, arg2, arg3, arg4, arg5)
#define bnx_log_debug6(level, log, err, fmt, arg1, arg2, arg3, arg4, arg5, arg6)
#define bnx_log_debug7(level, log, err, fmt, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
#define bnx_log_debug8(level, log, err, fmt, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)


#endif

/*************************************/

bnx_log_t *bnx_log_init(u_char *prefix, u_char *error_log);
void bnx_cdecl bnx_log_abort(bnx_err_t err, const char *fmt, ...);
void bnx_cdecl bnx_log_stderr(bnx_err_t err, const char *fmt, ...);
u_char *bnx_log_errno(u_char *buf, u_char *last, bnx_err_t err);
bnx_int_t bnx_log_open_default(bnx_cycle_t *cycle);
bnx_int_t bnx_log_redirect_stderr(bnx_cycle_t *cycle);
bnx_log_t *bnx_log_get_file_log(bnx_log_t *head);
char *bnx_log_set_log(bnx_conf_t *cf, bnx_log_t **head);


/*
 * bnx_write stderr() cannot be implemented as macro, since
 * MSVC does not allow to use #ifdef inside macro parameters.
 * 
 * bnx_write_fd() is used instead of bnx_write_console(), since
 * CharToOemBuff() inside bnx_write_console() cannot be used with
 * read only buffer as destination and CharToOmenBuff() is not needed
 * for bnx_write_stderr() anyway
 */
static bnx_inline void
bnx_write_stderr(char *text){
    (void) bnx_write_fd(bnx_stdout, text, bnx_strlen(text));
}

extern bnx_modele_t bnx_errlog_modules;
extern bnx_uint_t   bnx_use_stderr;


#endif  /* _BNX_LOG_H_INCLUDED_ */