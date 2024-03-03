#include "./core.h"
#include "./config.h"

int main(void)
{
    printf("bebnex is running...\n");
    bnx_string_t path = bnx_create_string("./root_dev/etc/bebnex/bebnex.conf\0");
    bnx_conf_t conf = bnx_read_conf(path);
    
    bnx_socket_t sock = bnx_create_socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in_t sin = bnx_create_sockaddr_in(AF_INET, INADDR_ANY, conf.port);
    bnx_bind_socket(sock.fd, sin);
    bnx_listen_socket(sock.fd, conf.max_con);
    bnx_launch(sock, sin);
    close(sock.fd);
}
