#include "bnx_config.h"
#include "bnx_core.h"
#include "bebnex.h"

#include "bnx_cycle.h"

static void bnx_show_version_info(void);
static bnx_int_t bnx_add_inherited_sockets(bnx_cycle_t *cycle);
static void bnx_cleanup_environment(void *data);
static bnx_int_t bnx_get_options(int argc, char *const *argv);
static bnx_int_t bnx_process_options(bnx_cycle_t *cycle);
static bnx_int_t bnx_save_argv(bnx_cycle_t *cycle, int argc, char *const *argv);
static void *bnx_core_module_create_conf(bnx_cycle_t *cycle, int argc, char *const *argv);
static char *bnx_core_module_init_conf(bnx_cycle_t *cycle, void *conf);
/*
static char *bnx_set_user(bnx_conf_t *cf, bnx_command_t *cmd, void *conf);
static char *bnx_set_env(bnx_conf_t *cf, bnx_command_t *cmd, void *conf);
static char *bnx_set_priority(bnx_conf_t *cf, bnx_command_t *cmd, void *conf);
static char *bnx_set_cpu_affinity(bnx_conf_t *cf, bnx_command_t *cmd, void *conf);
static char *bnx_set_worker_process(bnx_conf_t *cf, bnx_command_t *cmd, void *conf);
static char *bnx_load_module(bnx_conf_t *cf, bnx_command_t *cmd, void *conf);
#if (BNX_HAVE_DLOPEN)
static void bnx_unload_module(void *data);
#endif
*/



/* cycle_tのエラーを潰した後で、以下のconf_enum_tのエラーを潰す。
static bnx_conf_enum_t bnx_debug_points[]={
    {bnx_string("stop"), BNX_DEBUG_POINT_STOP},
    {bnx_string("abort"), BNX_DEBUG_POINTS_ABORT },
    {bnx_full_string,0}
};
*/


#define STR_LEN 128
static char bnx_command[][STR_LEN] = {"start", "stop", "version"};

int main(int argc, char *argv[]){
    /*
        bebnex start : command to start bebnex
        bebnex stop  : command to stop bebnex
        bebnex version : command to see version
    */

    int command_invalid = BNX_ERROR;

    if (strcmp(argv[1],"start") == 0){
        printf("starting bebnex...\n");
        command_invalid = BNX_OK;
    }
    
    if (strcmp(argv[1],"stop") == 0){
        printf("stopping bebnex...\n");  
        command_invalid = BNX_OK;
    }
    
    if(strcmp(argv[1],"version")==0){
        printf("version : %s\n",BNX_VERSION);
        command_invalid = BNX_OK;
    }

    if (command_invalid){
        printf("Invalid command\n");
    }

    return command_invalid;
}