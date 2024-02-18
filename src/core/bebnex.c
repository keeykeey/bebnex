#include "./core.h"
#include "./config.h"

int main(void)
{
    printf("hello bebnex!\n");
    int port = 8080; // TODO: read from setting file
    int max_con = 10; // TODO: read from setting file
    
    bnx_socket_t sock = bnx_create_socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in_t sin = bnx_create_sockaddr_in(AF_INET, INADDR_ANY, port);
    bnx_bind_socket(sock.fd, sin);
    bnx_listen_socket(sock.fd, max_con);
    bnx_launch(sock, sin);
    close(sock.fd);
}
