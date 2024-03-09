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
        // TODO: it's just a mock and has to be rewrited
        char key[256];
        int kpos = 0;
        char value[256];
        int vpos = 0;
        int readk = 1;
        int readv = 0;
        while ( (ch = fgetc(fp)) != EOF) {
            if ( ch == '=') {
                key[kpos] = '\0';
                readk = 0;
                readv = 1;
                kpos = 0;
                vpos = 0;
                continue;
            } else if (ch == '\n') {
                value[vpos] = '\0';
                readk = 1;
                readv = 0;
                kpos = 0;
                vpos = 0;

                if (strcmp(key, "PORT\0") == 0) {
                    bnx_string_t s = bnx_create_string(value);
                    bnx_uint_t u = bnx_atoui(s);

                    conf.port = bnx_atoui(bnx_create_string(value));
                } else if ( strcmp(key, "MAX_CON\0") == 0) {
                    bnx_string_t s = bnx_create_string(value);
                    bnx_uint_t u = bnx_atoui(s);

                    conf.max_con = bnx_atoui(bnx_create_string(value));
                } else if (strcmp(key, "PREFIX\0") == 0) {
                    conf.prefix = bnx_create_string(value);
                }
                continue;
            }

            if (readk) {
                key[kpos++] = (char) ch;
            } else if (readv) {
                value[vpos++] = (char) ch;
            }
        };
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
