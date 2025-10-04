#include <stdio.h>
#include <stdlib.h>
#include "core/bnx_log.h"
#include "core/bnx_conf_file.h"

int main(void)
{
    BNX_LOG_FATAL("fatal(%d)", 1);
    BNX_LOG_ERROR("error(%d)", 1);
    BNX_LOG_INFO("info(%d)", 104);
    BNX_LOG_DEBUG("debug(%d)", 104);
    BNX_LOG_INFO("hello %s", "world");

    FILE *fp;
    fp = tmpfile();
    if (fp == NULL) {
        perror("error");
        return 0;
    }
    fprintf(fp, "http {\n server { listen 80 }\n}\n https {\n server { listen 443 }\n}");
    fseek(fp, 0, SEEK_SET);
    
    bnx_conf_t *root = NULL;
    bnx_conf_init(&root, "root", 4);
    bnx_code_e result = bnx_conf_read(root, fp);
    if (result == BNX_EOF) {
        BNX_LOG_INFO("conf->children_count: (%lu)", root->children_count);

        fprintf(stdout, "conf.http: %s\n", root->children[0]->key);
        fprintf(stdout, "conf.https: %s\n", root->children[1]->key);
        bnx_conf_free(&root);
        fclose(fp);
        return 0;
    } else {
        bnx_conf_free(&root);
        fclose(fp);
        return 1;
    }
}
