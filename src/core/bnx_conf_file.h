#pragma once
#include <stdio.h>
#include <stdbool.h>
#include "core/bebnex.h"

#ifdef __cplusplus
extern "C" {
#endif

#define BNX_PREFIX_MAX_LENGTH 512
#define BNX_DEFAULT_PORT 80


/** environmental variable */
#ifndef BNX_PREFIX
#define BNX_PREFIX "/usr/local/bebnex"
#endif

#define BNX_DEFAULT_CONF_PATH "bebnex.conf"

/** configuration file definitions */
#define BNX_CONF_KEY_MAX_LENGTH   16
#define BNX_CONF_ROOT             "root"
#define BNX_CONF_HTTP             "http"
#define BNX_CONF_HTTPS            "https"
#define BNX_CONF_SERVER           "server"
#define BNX_CONF_LISTEN           "listen"
#define BNX_CONF_LOCATION         "location"
#define BNX_CONF_BLOCK_START      "{"
#define BNX_CONF_BLOCK_END        "}"
#define BNX_TOKEN_DELIM_SPACE     " "
#define BNX_TOKEN_DELIM_NEW_LINE  "\n"

typedef enum bnx_conf_token_kind {
    BNX_CONF_KEY,
    BNX_CONF_VALUE,
    BNX_CONF_RESERVED,
    BNX_CONF_NO_MATCH,
} bnx_conf_token_kind_e;

typedef struct bnx_conf bnx_conf_t;
struct bnx_conf {
    struct bnx_conf *parent;
    char key[BNX_CONF_KEY_MAX_LENGTH];

    char **value;
    size_t value_count;

    struct bnx_conf **children;
    size_t children_count;
};

typedef struct bnx_conf_pair bnx_conf_pair_t;
struct bnx_conf_pair {
    const char *parent;
    const char *child;
};

static const char *bnx_allowed_conf_key_array[BNX_CONF_KEY_MAX_LENGTH] = {
    BNX_CONF_ROOT,
    BNX_CONF_HTTP,
    BNX_CONF_HTTPS,
    BNX_CONF_SERVER,
    BNX_CONF_LISTEN,
    BNX_CONF_LOCATION,
    NULL,
};

static const char *bnx_reserved_conf_token[] = {
    BNX_CONF_BLOCK_START,
    BNX_CONF_BLOCK_END,
    BNX_TOKEN_DELIM_SPACE,
    BNX_TOKEN_DELIM_NEW_LINE,
    NULL,
};

static const bnx_conf_pair_t bnx_allowed_conf_pair[] = {
    { BNX_CONF_ROOT, BNX_CONF_HTTP },
    { BNX_CONF_ROOT, BNX_CONF_HTTPS },
    { BNX_CONF_HTTP, BNX_CONF_SERVER },
    { BNX_CONF_HTTPS, BNX_CONF_SERVER },
    { BNX_CONF_SERVER, BNX_CONF_LISTEN },
    { BNX_CONF_SERVER, BNX_CONF_LOCATION },
};

bnx_code_e bnx_read_token(FILE *fp, char *buf, size_t size);
bnx_conf_token_kind_e bnx_conf_token_kind(const char *token);
bnx_code_e bnx_conf_allowed_key(const char *ch, const char *allowed_keys[], size_t arr_size);
bnx_code_e bnx_conf_reserved_token(const char *token, const char *reserved_token[], size_t arr_size);

bnx_code_e bnx_conf_init(bnx_conf_t **conf, const char *key, size_t key_length);
bnx_code_e bnx_conf_free(bnx_conf_t **conf);
bnx_code_e bnx_conf_add_child(bnx_conf_t *conf, bnx_conf_t *child);
bnx_code_e bnx_conf_add_value(bnx_conf_t *conf, const char *value);
bnx_code_e bnx_conf_valid_pair(bnx_conf_t *parent, bnx_conf_t *child, const bnx_conf_pair_t allowed[], size_t size);

bnx_code_e bnx_conf_read(bnx_conf_t *conf, FILE *fp);
bnx_code_e bnx_conf_check_valid_file(FILE *fp);


#ifdef __cplusplus
}
#endif
