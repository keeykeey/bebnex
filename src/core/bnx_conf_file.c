#include "./core.h"
#include "./config.h"

bnx_conf_t bnx_read_conf(bnx_string_t path)
{
    bnx_conf_t conf;

    FILE *fp;
    if ((fp = fopen(path.data, "r")) == NULL) {
        fprintf(stderr, BNX_ERROR_MESSAGE);
    } else {
        conf.port = get_bnx_conf_port(fp);
        conf.max_con = get_bnx_conf_max_con(fp);
        conf.prefix = get_bnx_conf_prefix(fp);
    }

    fclose(fp);

    return conf;
}

bnx_uint_t get_bnx_conf_port(FILE *fp)
{
    return 8080;
}

bnx_uint_t get_bnx_conf_max_con(FILE *fp)
{
    return 10;
}

bnx_string_t get_bnx_conf_prefix(FILE *fp)
{
    bnx_string_t s = bnx_create_string("/root_dev/var/www/index.html\0");

    return s;
}
