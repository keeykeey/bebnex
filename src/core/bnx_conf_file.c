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
                ps.key[ps.kpos++] = (char) ch;
            } else if (ps.readv) {
                ps.value[ps.vpos++] = (char) ch;
            }
        };
    }

    fclose(fp);

    return conf;
}

bnx_conf_parse_state_t create_init_bnx_conf_parse_state()
{
    bnx_conf_parse_state_t conf;
    conf.key[0] = '\0';
    conf.value[0] = '\0';
    conf.kpos = 0;
    conf.vpos = 0;
    conf.readk = 1;
    conf.readv = 0;

    return conf;
}

void set_bnx_conf(bnx_conf_t *conf, bnx_conf_parse_state_t *ps)
{
    if (strcmp(ps->key, "PORT\0") == 0) {
        bnx_string_t s = bnx_create_string(ps->value);
        bnx_uint_t u = bnx_atoui(s);

        conf->port = bnx_atoui(bnx_create_string(ps->value));
    } else if ( strcmp(ps->key, "MAX_CON\0") == 0) {
        bnx_string_t s = bnx_create_string(ps->value);
        bnx_uint_t u = bnx_atoui(s);

        conf->max_con = bnx_atoui(bnx_create_string(ps->value));
    } else if (strcmp(ps->key, "PREFIX\0") == 0) {
        conf->prefix = bnx_create_string(ps->value);
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