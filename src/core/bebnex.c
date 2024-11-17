#include "./core.h"
#include "./config.h"

bnx_int_t bnx_create_pidfile(bnx_pid_t pid, bnx_logger_t *errlogger);

bnx_pid_t pid;
bnx_logger_t accesslogger;
bnx_logger_t errlogger;
bnx_string_t conf_file_path;
bnx_conf_t conf;

int main(void)
{
    fprintf(stdout, "bebnex is running...\n");

    pid = bnx_getpid();
    bnx_init_logger(&accesslogger, bnx_write_log, BNX_ACCESS_LOG_FILE);
    bnx_init_logger(&errlogger, bnx_write_log, BNX_ERROR_LOG_FILE);
    conf_file_path = bnx_create_string(BNX_CONF_FILE_PATH);
    conf = bnx_read_conf(conf_file_path, &errlogger);

    if (bnx_create_pidfile(pid, &errlogger) != BNX_OK) {
        return BNX_ERROR;
    };

/** initialize Winsock DLL (windows platform only) */
#ifdef BNX_WIN32
    WSADATA d;
    if (WSAStartup(MAKEWORD(2, 2), &d)) {
        return BNX_ERROR;
    }
#endif /** BNX_WIN32 */

#ifdef BNX_IPV6
    bnx_socket_t fd = bnx_socket(PF_INET6, SOCK_STREAM, IPPROTO_TCP, &errlogger);
    struct sockaddr_in6 sin;
    sin.sin6_family = AF_INET6;
    sin.sin6_port = htons(conf.port);
    sin.sin6_addr = in6addr_any;
#else
    bnx_socket_t fd = bnx_socket(PF_INET, SOCK_STREAM, IPPROTO_TCP, &errlogger);
    struct sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(conf.port);
    sin.sin_addr.s_addr = INADDR_ANY;
#endif /** BNX_IPV6 */
    
    bnx_listening_t *ls = bnx_create_listening((struct sockaddr *)&sin, sizeof(sin));

#ifdef BNX_IPV6
    int option = 0;
    if (setsockopt(fd, IPPROTO_IPV6, IPV6_V6ONLY, (void *)&option, sizeof(option)))
    {
        char *ch;
        sprintf(ch, "setsockopt() failed. (%d)\n", bnx_get_socket_errno());
        errlogger.fwriter(bnx_create_error_log_message(ch), &errlogger);
    }
#endif /** BNX_IPV6 */

    bnx_bind_socket(fd, ls, &errlogger);
    bnx_listen_socket(ls, &errlogger);
    bnx_launch(ls, conf, &accesslogger);
    bnx_close_socket(ls->fd);
}

bnx_int_t
bnx_create_pidfile(bnx_pid_t pid, bnx_logger_t *errlogger)
{
    /** 
     * File name is supposed to be pid.bnx.
     * And returning value of 'cat pid.bnx' should be process id of bebnex.
     **/
    FILE *fp;

    char *fname = calloc(1024, sizeof(char));
    snprintf(fname, 1024, "%s/log/pid.bnx", BNX_PREFIX);

    if ((fp = fopen(fname, "w")) == NULL) {
        errlogger->fwriter(bnx_create_error_log_message("failed to create pid file"), errlogger);

        free(fname);
        return BNX_ERROR;

    } else {
        fprintf(fp, "%d", pid);
        free(fname);
        fclose(fp);
        return BNX_OK;
    }
}
