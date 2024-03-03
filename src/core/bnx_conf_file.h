#ifndef BNX_CONF_FILE
#define BNX_CONF_FILE

#define BNX_CONF_FILE_PATH "root_dev/etc/bebnex/"
#define BNX_CONF_FILE_NAME "bebnex.conf"

typedef struct bnx_conf_s {
    bnx_uint_t port;
    bnx_uint_t max_con;
    bnx_string_t prefix; 
} bnx_conf_t;

bnx_conf_t bnx_read_conf(bnx_string_t path);
bnx_uint_t get_bnx_conf_port(FILE *fp);
bnx_uint_t get_bnx_conf_max_con(FILE *fp);
bnx_string_t get_bnx_conf_prefix(FILE *fp);

#endif /** BNX_CONF_FILE */