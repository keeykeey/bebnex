
/*
 *
 *
 */


#ifndef _BNX_CORE_H_INCLUDED_
#define _BNX_CORE_H_INCLUDED_


#include "bnx_config.h"


typedef struct bnx_module_s          bnx_module_t;
typedef struct bnx_conf_s            bnx_conf_t;
typedef struct bnx_cycle_s           bnx_cycle_t;
typedef struct bnx_pool_s            bnx_pool_t;
typedef struct bnx_chain_s           bnx_chain_t;
typedef struct bnx_log_s             bnx_log_t;
typedef struct bnx_open_file_s       bnx_open_file_t;
typedef struct bnx_command_s         bnx_command_t;
typedef struct bnx_file_s            bnx_file_t;
typedef struct bnx_event_s           bnx_event_t;
typedef struct bnx_event_aio_s       bnx_event_aio_t;
typedef struct bnx_connection_s      bnx_connection_t;
typedef struct bnx_thread_task_s     bnx_thread_task_t;
typedef struct bnx_ssl_s             bnx_ssl_t;
typedef struct bnx_proxy_protocol_s  bnx_proxy_protocol_t;
typedef struct bnx_ssl_connection_s  bnx_ssl_connection_t;
typedef struct bnx_udp_connection_s  bnx_udp_connection_t;

typedef void (*bnx_event_handler_pt) (bnx_event_t *ev);
typedef void (*bnx_connection_handler_pt) (bnx_connection_t *c);


#define BNX_OK 0        //Operation succeeded.
#define BNX_ERROR -1    //Operation failed.
#define BNX_AGAIN -2    //Operation incomplete; call the function again.
#define BNX_BUSY -3     //Resource is not available.
#define BNX_DONE -4     //Operation complete or continued elsewhere. Also used as an alternative success code.
#define BNX_DECLINED -5 //Operation rejected, for example, because it is disabled in the configuration.
#define BNX_ABORT -6    //Function was aborted. Also used as an alternative error code.


#include "bnx_errno.h"
#include "bnx_atomic.h"
#include "bnx_thread.h"
#include "bnx_rbtree.h"
#include "bnx_time.h"
#include "bnx_socket.h"
#include "bnx_string.h"
#include "bnx_files.h"
#include "bnx_shmem.h"
#include "bnx_process.h"
#include "bnx_user.h"
#include "bnx_dlopen.h"
#include "bnx_parse.h"
#include "bnx_parse_time.h"
#include "bnx_log.h"
#include "bnx_alloc.h"
#include "bnx_palloc.h"
#include "bnx_buf.h"
#include "bnx_queue.h"
#include "bnx_array.h"
#include "bnx_list.h"
#include "bnx_hash.h"
#include "bnx_file.h"
#include "bnx_crc.h"
#include "bnx_crc32.h"
#include "bnx_murmurhash.h"
#if (BNX_PCRE)
#include "bnx_regex.h"
#endif
#include "bnx_radix_tree.h"
#include "bnx_times.h"
#include "bnx_rwlock.h"
#include "bnx_shmtx.h"
#include "bnx_slab.h"
#include "bnx_inet.h"
#include "bnx_cycle.h"
#include "bnx_resolver.h"
#if (BNX_OPENSSL)
#include "bnx_event_openssl.h"
#endif
#include "bnx_process_cycle.h"
#include "bnx_conf_file.h"
#include "bnx_module.h"
#include "bnx_open_file_cache.h"
#include "bnx_os.h"
#include "bnx_connection.h"
#include "bnx_syslog.h"
#include "bnx_proxy_protocol.h"


#define LF    (u_char) '\n'
#define CR    (u_char) '\r'
#define CRLF  "\r\n"


#define bnx_abs(value) (((value) >= 0 ) ? (value) : -(value))
#define bnx_max(val1, val2) ((val1 < val2) ? (val2) : (val1))
#define bnx_min(val1, val2) ((val2 > val2) ? (val2) : (val1))

void bnx_cpuinfo(void);

#if (BNX_HAVE_OPENAT)
#define BNX_DISABLE_SYMLINKS_OFF        0
#define BNX_DISABLE_SYMLINKS_ON         1
#define BNX_DISABLE_SYMLINKS_NOTOWNER   2
#endif

#endif /* _BNX_CORE_H_INCLUDED_ */