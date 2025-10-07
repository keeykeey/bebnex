#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "core/bnx_conf_file.h"
#include "core/bnx_log.h"

bnx_code_e bnx_read_token(FILE *fp, char *buf, size_t size)
{
    if (fp == NULL || buf == NULL || size == 0) {
        return BNX_INVALID_ARGUMENT;
    }

    int ch;
    bool found = false;
    size_t i = 0;

    for (;;) {
        if (i >= (size - 1)) {
            buf[i] = '\0';
            return BNX_STOP;
        }

        ch = fgetc(fp);

        if (ch == EOF) {
            buf[i] = '\0';
            return BNX_EOF;
        }

        if (ch == ' ' || ch == '\n' || ch =='\t') {
            if (found) {
                buf[i] = '\0';
                return BNX_OK;
            } else {
                continue;
            }
        }

        buf[i++] = (char)ch;
        found = true;
    }
}

bnx_conf_token_kind_e bnx_conf_token_kind(const char *token)
{

    if (!token) {
        return BNX_CONF_NO_MATCH;
    }

    bnx_code_e result;
    size_t key_array_length = sizeof(bnx_allowed_conf_key_array) / sizeof(bnx_allowed_conf_key_array[0]);

    result = bnx_conf_allowed_key(token, bnx_allowed_conf_key_array, key_array_length);
    if (result == BNX_OK) {
        return BNX_CONF_KEY;
    }

    size_t reserved_token_array_length = sizeof(bnx_reserved_conf_token) / sizeof(bnx_reserved_conf_token[0]);
    result = bnx_conf_reserved_token(token, bnx_reserved_conf_token, reserved_token_array_length);
    if (result == BNX_OK) {
        return BNX_CONF_RESERVED;
    }

    return BNX_CONF_VALUE;
}

bnx_code_e bnx_conf_allowed_key(const char *ch, const char *allowed_keys[], size_t arr_size)
{
    if (!ch) {
        return BNX_INVALID_ARGUMENT;
    }
    for (size_t i = 0; i < arr_size; ++i) {
        if (allowed_keys[i] == NULL) {
            return BNX_ERROR;
        }

        if (strncmp(ch, allowed_keys[i], BNX_CONF_KEY_MAX_LENGTH) == 0) {
            return BNX_OK;
        }
    }

    return BNX_ERROR;
}

bnx_code_e bnx_conf_reserved_token(const char *token, const char *reserved_tokens[], size_t arr_size)
{
    if (!token) return BNX_INVALID_ARGUMENT;

    for (size_t i = 0; i < arr_size; ++i) {
        if (reserved_tokens[i] == NULL) {
            return BNX_ERROR;
        }
        if (strncmp(token, reserved_tokens[i], strlen(token)) == 0) {
            return BNX_OK;
        }
    }

    return BNX_ERROR;
}

bnx_code_e bnx_conf_init(bnx_conf_t **out_conf, const char *key, size_t key_length)
{

    if (!out_conf) return BNX_INVALID_ARGUMENT;

    bnx_conf_t *conf = calloc(1, sizeof(bnx_conf_t));
    if (!conf) return BNX_ERROR;

    conf->parent = NULL;

    if (key && key_length > 0) {
        size_t copy_len = (key_length < BNX_CONF_KEY_MAX_LENGTH - 1)
          ? key_length
          : BNX_CONF_KEY_MAX_LENGTH - 1;
        
        memcpy(conf->key, key, copy_len);
        conf->key[copy_len] = '\0';
    } else {
        conf->key[0] = '\0';
    }

    *out_conf = conf;

    return BNX_OK;
}

bnx_code_e bnx_conf_free(bnx_conf_t **conf_ptr)
{
    if (!conf_ptr || !*conf_ptr) return BNX_INVALID_ARGUMENT;

    bnx_conf_t *conf = *conf_ptr;
    if (conf->value) {
        for (size_t i = 0; i < conf->value_count; i++) {
            free(conf->value[i]);
            conf->value[i] = NULL;
        }
        free(conf->value);
    }

    if (conf->children) {
        for (size_t i = 0; i < conf->children_count; i++) {
            bnx_conf_free(&(conf->children[0]));
        }
        free(conf->children);
        conf->children = NULL;
    }

    free(conf);
    *conf_ptr = NULL;

    return BNX_OK;
}

bnx_code_e bnx_conf_add_child(bnx_conf_t *conf, bnx_conf_t *child)
{
    if (!conf || !child || child->parent) return BNX_INVALID_ARGUMENT;

    bnx_conf_t **new_children = realloc(
        conf->children,
        (conf->children_count + 1) * sizeof(bnx_conf_t*)
    );
    if (!new_children) return BNX_MEMORY_ERROR;

    conf->children = new_children;
    conf->children[conf->children_count] = child;
    ++(conf->children_count);
    child->parent = conf;

    return BNX_OK;
}

bnx_code_e bnx_conf_add_value(bnx_conf_t *conf, const char *value)
{
    if (!conf) return BNX_INVALID_ARGUMENT;

    char **new_value = realloc(
        conf->value,
        (conf->value_count + 1) * sizeof(*conf->value)
    );

    if (!new_value) return BNX_MEMORY_ERROR;

    conf->value = new_value;
    conf->value[conf->value_count] = strdup(value);
    ++(conf->value_count);

    return BNX_OK;
}

bnx_code_e bnx_conf_valid_pair(bnx_conf_t *parent, bnx_conf_t *child, const bnx_conf_pair_t allowed[], size_t size)
{
    if (!parent || !child) return BNX_INVALID_ARGUMENT;

    for (size_t i = 0; i < size; ++i)
    {
        if (
            strncmp(parent->key, allowed[i].parent, BNX_CONF_KEY_MAX_LENGTH) == 0
            && strncmp(child->key, allowed[i].child, BNX_CONF_KEY_MAX_LENGTH) == 0
        ) {
            return BNX_OK;
        }
    }

    return BNX_ERROR;
}

bnx_code_e bnx_conf_read(bnx_conf_t *root_conf, FILE *fp)
{
    if (root_conf == NULL || fp == NULL) {
        BNX_LOG_ERROR("failed to read config file (%d)", BNX_INVALID_ARGUMENT);
        return BNX_INVALID_ARGUMENT;
    }

    char token[BNX_CONF_KEY_MAX_LENGTH] = {0};
    bnx_conf_t *current = root_conf;
    bnx_conf_t *current_block = root_conf;

    for (;;) {
        bnx_code_e code = bnx_read_token(fp, token, sizeof(token));
        if (code == BNX_OK) {
            bnx_conf_token_kind_e token_kind = bnx_conf_token_kind(token);

            bnx_code_e c;
            switch (token_kind) {
                case BNX_CONF_KEY:
                    bnx_conf_t *child_conf = NULL;
                    c = bnx_conf_init(&child_conf, token, strlen(token));
                    if (c != BNX_OK) {
                        return c;
                    } else {
                        size_t size = sizeof(bnx_allowed_conf_pair) / sizeof(bnx_allowed_conf_key_array[0]);
                        c = bnx_conf_valid_pair(current, child_conf, bnx_allowed_conf_pair, size);
                        if (c == BNX_OK) {
                            c = bnx_conf_add_child(current, child_conf);
                            if (c != BNX_OK) {
                                return c;
                            } else {
                                // go to next loop.
                                current = child_conf;
                                break;
                            }
                        } else {
                            return c;
                        }
                    }
                case BNX_CONF_VALUE:
                    c = bnx_conf_add_value(current, token);
                    if (c == BNX_OK) {
                        // go to next loop.
                        break;
                    } else {
                        return c;
                    }
                case BNX_CONF_RESERVED:
                    if (strncmp(token, BNX_CONF_BLOCK_START, strlen(token)) == 0) {
                        current_block = current;
                    } else if (strncmp(token, BNX_CONF_BLOCK_END, strlen(token)) == 0) {
                        if (!current_block || !current_block->parent) {
                            BNX_LOG_ERROR("failed to read config file (%d)", BNX_ERROR);
                            return BNX_ERROR;
                        } else {
                            bnx_conf_t *tmp = current_block->parent;
                            current = tmp;
                            current_block = tmp;
                        }
                    }
                    break;
                case BNX_CONF_NO_MATCH:
                    BNX_LOG_ERROR("fail to read config file. couldn't parse unexpected syntax (%d)", BNX_RUNTIME_ERROR);
                    return BNX_RUNTIME_ERROR;
            }
        } else {
            BNX_LOG_ERROR("read token failed while reading conf-file (%d)", code);
            return code;
        }
    }
}

bnx_code_e bnx_conf_check_valid_file(FILE *fp)
{
    if (!fp) return BNX_ERROR;

    int block_start_count = 0;
    int block_end_count = 0;

    int ch;
    int before;
    do {
        if (ch) before = ch;
        ch = fgetc(fp);

        if (ch == '{')  block_start_count += 1;
        if (ch == '}')  block_end_count += 1;

    } while(ch != EOF);

    if (block_start_count != block_end_count) {
        BNX_LOG_ERROR("Conf-file syntax error(%d). Found %d '{' but found %d '}'", BNX_ERROR, block_start_count, block_end_count);
        return BNX_ERROR;
    }

    if (before != '\n') {
        BNX_LOG_ERROR("Conf-file syntax error(%d). conf-file is expected to end with \\n", BNX_ERROR);
        return BNX_ERROR;
    }

    return BNX_OK;
}
