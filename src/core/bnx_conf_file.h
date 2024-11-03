#ifndef BNX_CONF_FILE_H
#define BNX_CONF_FILE_H

#include "./config.h"
#include "./core.h"

#define CONF_KEY_LEN 256
#define CONF_VALUE_LEN  256

typedef struct bnx_conf_s {
    bnx_uint_t port;
    bnx_string_t prefix; 
} bnx_conf_t;

typedef struct bnx_conf_pares_state_s {
    char key[CONF_KEY_LEN];
    char value[CONF_VALUE_LEN];
    bnx_uint_t kpos;
    bnx_uint_t vpos;
    bnx_uint_t readk; // TODO: boolean
    bnx_uint_t readv; // TODO: boolean
} bnx_conf_parse_state_t;

bnx_conf_t bnx_read_conf(bnx_string_t path);
bnx_conf_parse_state_t create_init_bnx_conf_parse_state();
void set_bnx_conf(bnx_conf_t *conf, bnx_conf_parse_state_t *state);
void terminate_parse_bnx_conf_key(bnx_conf_parse_state_t *state);
void terminate_parse_bnx_conf_value(bnx_conf_parse_state_t *state);

#endif /** BNX_CONF_FILE_H */