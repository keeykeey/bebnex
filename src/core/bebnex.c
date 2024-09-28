#include "./core.h"
#include "./config.h"

bnx_logger_t logger;

int main(void)
{
    bnx_init_logger(&logger, bnx_write_log, BNX_ACCESS_LOG_FILE);

    fprintf(stdout, "bebnex is running...\n");
    bnx_string_t conf_file_path = bnx_create_string(BNX_CONF_FILE_PATH);
    bnx_conf_t conf = bnx_read_conf(conf_file_path);

/** initialize Winsock DLL (windows platform only) */
#ifdef BNX_WIN32
    WSADATA d;
    if (WSAStartup(MAKEWORD(2, 2), &d)) {
        fprintf(stderr, "[error] Failed to initialize Winsock DLL\n");
        return BNX_NG;
    }
#endif /** BNX_WIN32 */
    bnx_socket_t fd = bnx_socket(PF_INET, SOCK_STREAM, IPPROTO_TCP, &logger);

    struct sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(conf.port);
    sin.sin_addr.s_addr = INADDR_ANY;
    
    bnx_listening_t *ls = bnx_create_listening((struct sockaddr *)&sin, sizeof(sin));
    bnx_bind_socket(fd, ls);
    bnx_listen_socket(ls);
    bnx_launch(ls, conf, &logger);
    bnx_close_socket(ls->fd);
}
