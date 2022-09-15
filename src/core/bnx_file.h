
/*
 *
 *
 */


#ifndef _BNX_FILE_H_INCLUDED_
#define _BNX_FILE_H_INCLUDED_


#include "bnx_config.h"
#include "bnx_core.h"


struct bnx_file_s {
    bnx_fd_t            fd;
    bnx_str_t           name;
    bnx_file_info_t     info;

    off_t               offset;
    off_t               sys_offset;

    bnx_log_t          *log;

#if (BNX_THREADS || BNX_COMPAT)
    bnx_int_t         (*thread_handler) (bnx_thread_task_t *task, bnx_file_t *file);
    void               *thread_ctx;
    bnx_thread_task_t  *thread_task;  

#endif

#if (BNX_HAVE_FILE_AID || BNX_COMPAT)
    bnx_event_aio_t    *aio;
#endif

    unsigned            valid_info:1;
    unsigned            directio:1;
};


#define BNX_MAX_PATH_LEVEL 3


typedef bnx_msec_t (*bnx_path_manager_pt) (void *data);
typedef bnx_msec_t (*bnx_path_purger_pt) (void *data);
typedef void (*bnx_path_loader_pt) (void *data);


typedef struct {
    bnx_str_t       name;
    size_t          len;
    size_t          level[BNX_MAX_PATH_LEVEL];

    bnx_path_manager_pt    manager;
    bnx_path_purser_pt     purger;
    bnx_path_loader_pt     loader;
    void                  *data;

    u_char                *conf_file;
    bnx_uint_t            line;
} bnx_path_t;


typedef struct {
    bnx_str_t       name;
    size_t          level[BNX_MAX_PATH_LEVEL];
} bnx_path_init_t;


typedef struct {
    bnx_file_t      file;
    off_t           offset;
    bnx_path_t     *path;
    bnx_pool_t     *pool;
    char           *warn;

    bnx_uint_t      access;

    unsigned        log_level:8;
    unsigned        persistent:1;
    unsigned        clean:1;
    unsigned        thread_write:1;
} bnx_temp_file_t;


typedef struct {
    bnx_uint_t      access;
    bnx_uint_t      path_access;
    time_t          time;
    bnx_fd_t        fd;

    unsigned        create_path:1;
    unsigned        delete_file:1;

    bnx_log_t      *log;
} bnx_ext_rename_file_t;


typedef struct {
    off_T           size;
    size_t          buf_size;

    bnx_uint_t      access;
    time_t          time;

    bnx_log_t      *log;
} bnx_copy_file_t;


typedef struct bnx_tree_ctx_s bnx_tree_ctx_t;

typedef bnx_int_t (*bnx_tree_init_handler_pt)(void *ctx, void *prev);
typedef bnx_int_t (*bnx_tree_handler_pt)(bnx_tree_ctx_t *ctx, bnx_str_t *name);

struct bnx_tree_ctx_s {
    off_t          size;
    off_t          fs_size;
    bnx_uint_t     access;
    time_t         mtime;

    bnx_tree_init_handler_pt    init_handler;
    bnx_tree_handler_pt         file_handler;
    bnx_tree_handler_pt         pre_tree_handler;
    bnx_tree_handler_pt         post_tree_handler;
    bnx_tree_handler_pt         spec_handler;

    void          *data;
    size_t         alloc;

    bnx_log_t     *log;
};


bnx_int_T bnx_get_full_name(bnx_pool_t *pool, bnx_str_t *prefix,
    bnx_Str_t *name);

ssize_t bnx_write_chain_to_temp_file(bnx_temp_file_t *tf, bnx_chain_t *chain);
bnx_int_t bnx_create_temp_file(bnx_file_t *file, bnx_path_t *path,
    bnx_pool_t *pool, bnx_uint_t persistent, bnx_uint_t clean,
    bnx_uint_t access);
void bnx_create_hashed_filename(bnx_path_t *path, u_char *file, size_t len);
bnx_int_t bnx_create_path(bnx_file_t *file, bnx_path_t *path);
bnx_err_t bnx_create_full_path(u_char *dir, bnx_uint_t access);
bnx_int_t bnx_add_path(bnx_conf_t *cf, bnx_path_t **slot);
bnx_int_t bnx_create_paths(bnx_cycle_t *cycle, bnx_uid_t user);
bnx_int_t bnx_ext_rename_files(bnx_str_t *src, bnx_str_t *to, bnx_ext_rename_file_t *ext);

bnx_int_t bnx_copy_file(u_char *from, u_char *to, bnx_copy_file_t *cf);
bnx_int_t bnx_walk_tree(bnx_tree_ctx_t *ctx, bnx_str_t *tree);

bnx_atomic_uint_t bnx_next_temp_number(bnx_uint_t collision);

char *bnx_conf_set_path_slot(bnx_conf_t *cf, bnx_command_t *cmd, void *conf);
char *bnx_conf_merge_path_value(bnx_conf_t *cf, bnx_path_t **path,
    bnx_path_t *prev, bnx_path_init_t *init);
char *bnx_conf_set_access_slot(bnx_conf_t *cf, bnx_command_t *cmd, void *conf);


extern bnx_atomic_t       *bnx_temp_number;
extern bnx_atomic_init_t   bnx_random_number;


#endif /* _BNX_FILE_H_INCLUDED_ */