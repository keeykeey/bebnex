#include "./core.h"
#include "./config.h"

bnx_conf_t bnx_read_conf(bnx_string_t path)
{
    bnx_conf_t conf;

    FILE *fp;
    int ch;
    if ((fp = fopen(path.data, "r")) == NULL) {
        fprintf(stderr, BNX_ERROR_MESSAGE);
    } else {
        bnx_conf_parse_state_t ps = create_init_bnx_conf_parse_state();

        while ( (ch = fgetc(fp)) != EOF) {
            if ( ch == '=') {
                terminate_parse_bnx_conf_key(&ps);
                continue;
            } else if (ch == '\n') {
                terminate_parse_bnx_conf_value(&ps);
                set_bnx_conf(&conf, &ps);
                continue;
            }

            if (ps.readk) {
                ps.key[(ps.kpos)++] = (char) ch;
            } else if (ps.readv) {
                ps.value[(ps.vpos)++] = (char) ch;
            }
        };
    }

    fclose(fp);

    return conf;
}

bnx_conf_parse_state_t create_init_bnx_conf_parse_state()
{
    bnx_conf_parse_state_t ps;
    ps.key[0] = '\0';
    ps.value[0] = '\0';
    ps.kpos = 0;
    ps.vpos = 0;
    ps.readk = 1;
    ps.readv = 0;

    return ps;
}

void set_bnx_conf(bnx_conf_t *conf, bnx_conf_parse_state_t *ps)
{
    char *dst = calloc(bnx_str_len(ps->value), sizeof(char));
    bnx_copy_string(dst, ps->value);

    if (strcmp(ps->key, "PORT\0") == 0) {
        bnx_string_t port = bnx_create_string(dst);
        conf->port = bnx_atoui(port);
    } else if (strcmp(ps->key, "PREFIX\0") == 0) {
        conf->prefix = bnx_create_string(dst);
    }
}

void terminate_parse_bnx_conf_key(bnx_conf_parse_state_t *s)
{
    s->key[s->kpos] = '\0';
    s->readk = 0;
    s->readv = 1;
    s->kpos = 0;
    s->vpos = 0;
}

void terminate_parse_bnx_conf_value(bnx_conf_parse_state_t *s)
{
    s->value[s->vpos] = '\0';
    s->readk = 1;
    s->readv = 0;
    s->kpos = 0;
    s->vpos = 0;
}