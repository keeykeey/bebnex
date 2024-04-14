#include "./core.h"
#include "./config.h"

int main(void)
{
    printf("bebnex is running...\n");
    bnx_string_t path = bnx_create_string(BNX_CONF_FILE_PATH);
    bnx_conf_t conf = bnx_read_conf(path);

    bnx_socket_t fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    struct sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(conf.port);
    sin.sin_addr.s_addr = INADDR_ANY;
    
    bnx_listening_t *ls = bnx_create_listening((struct sockaddr *)&sin, sizeof(sin));
    bnx_bind_socket(fd, ls);
    bnx_listen_socket(ls);
    bnx_launch(ls, conf);
    close(ls->fd);
}
