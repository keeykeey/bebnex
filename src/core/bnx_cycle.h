
/*
 *
 *
 */


#ifndef _BNX_CYCLE_H_INCLUDED_
#define _BNX_CYCLE_H_INCLUDED_


#include "bnx_config.h"
#include "bnx_core.h"


#ifndef BNX_CYCLE_POOL_SIZE
#define BNX_CYCLE_POOL_SIZE      BNX_DEFAULT_POOL_SIZE
#endif


#define BNX_DEBUG_POINTS_STOP    1
#define BNX_DEBUG_POINTS_ABORT   2


typedef struct bnx_shm_zone_s    bnx_shm_zone_t;

typedef bnx_int_t (*bnx_shm_zone_init_pt) (bnx_shm_zone_t *zone, void *data);

struct bnx_shm_zone_s {
    void                    *data;
    bnx_shm_t                 shm;
    bnx_shm_zone_init_pt     init;
    void                    *tag;
    void                    *sync;
    bnx_uint_t              *noreuse;
};


struct bnx_cycle_s {
    void                    ****conf_ctx;
    bnx_pool_t                 *pool;

    bnx_log_t                  *log;
    bnx_log_t                   new_log;

    bnx_uint_t                  log_use_stderr;

    bnx_connection_t          **files;
    bnx_connection_t           *free_connections;
    bnx_uint_t                  free_connection_n;

    bnx_modules_t              **modules;
    bnx_uint_t                  modules_n;
    bnx_uint_t                  modules_used;

    bnx_queue_t                 reusable_connections_queue;
    bnx_uint_t                  reusable_connections_n;
    time_t                      connections_reuse_rime;

    bnx_array_t                 listening;
    bnx_array_t                 path;

    bnx_array_t                 config_dump;
    bnx_rbtree_t                config_dump_rbtree;
    bnx_rbtree_node_t           config_dump_sentinel;

    bnx_list_t                  open_files;
    bnx_list_t                  shared_memory;

    bnx_uint_t                  connection_n;
    bnx_uint_t                  files_n;

    bnx_connection_t           *connections;
    bnx_event_t                *read_events;
    bnx_event_t                *write_events;

    bnx_cycle_t                *old_cycle;

    bnx_str_t                   conf_file;
    bnx_str_t                   conf_param;
    bnx_str_t                   conf_prefix;
    bnx_str_t                   prefix;
    bnx_str_t                   error_log;
    bnx_str_t                   lock_file;
    bnx_str_t                   host_name;
};


typedef struct {
    bnx_flag_t                  damon;
    bnx_flag_t                  master;

    bnx_msec_t                  timer_resolution;
    bnx_msec_t                  shutdown_timeout;

    bnx_int_t                   worker_processes;
    bnx_int_t                   debug_points;

    bnx_int_t                   rlimit_nofile;
    off_t                       rlimit_core;

    int                         priority;

    bnx_uint_t                  cpu_affinity_auto;
    bnx_uint_t                  cpu_affinity_n;
    bnx_cpuset_t               *cpu_affinity;

    u_char                     *username;
    bnx_uid_t                   user;
    bnx_gid_t                   group;

    bnx_str_t                   working_directory;
    bnx_str_t                   lock_file;

    bnx_str_t                   pid;
    bnx_str_t                   oldpid;

    bnx_array_t                 env;
    char                      **environment;

    bnx_uint_t                  transparent;  
} bnx_core_conf_t;


#define bnx_is_init_cycle(cycle) (cycle->conf_ctx == NULL)


bnx_cycle_t *bnx_init_cycle(bnx_cycle_t *old_cycle);
bnx_int_t bnx_create_pidfile(bnx_str_t *name, bnx_log_t *log);
void bnx_delete_pidfile(bnx_cycle_t *cycle);
bnx_int_t bnx_signal_process(bnx_cycle_t *cycle, char *sig);
void bnx_reopen_files(bnx_cycle_t *cycle, bnx_uid_t user);
char **bnx_set_environment(bnx_cycle_t *cycle, bnx_uint_t *last);
bnx_pid_t bnx_exec_new_binary(bnx_cycle_t *cycle, char *const *argv);
bnx_cpuset_t *bnx_get_cpu_affinity(bnx_uint_t n);
bnx_shm_zone_t *bnx_shared_memory_add(bnx_conf_t *cf, bnx_str_t *name, size_t size, void *tag);
void bnx_set_shutdown_timer(bnx_cycle_t *cycle);



extern volatile bnx_cycle_t *bnx_cycle;
extern bnx_array_t           bnx_old_cycles;
extern bnx_module_t          bnx_core_module;
extern bnx_uint_t            bnx_test_config;
extern bnx_uint_t            bnx_dump_config;
extern bnx_uint_t            bnx_quiet_mode;


#endif /* _BNX_CYCLE_H_INCLUDED_ */