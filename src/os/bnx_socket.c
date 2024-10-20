#include "../core/core.h"
#include "../core/config.h"

bnx_socket_t bnx_socket(int __domain, int __type, int __protocol, bnx_logger_t *logger)
{
    bnx_socket_t fd = socket(__domain, __type, __protocol);
    if (!bnx_isvalid_socket(fd)) {
        int en = bnx_get_socket_errno();
        fprintf(stderr, "[error] socket() failed: (%d)\n", en);
        char *s = (char *)calloc(50, sizeof(char));
        sprintf(s,"socket() failed (%d)\n", en);
        bnx_string_t ls = bnx_create_string(s);
        logger->fwriter(ls, logger);
        free(s);
    }
    return fd;

}

bnx_socket_t bnx_accept(bnx_listening_t *ls, bnx_logger_t *logger)
{
    bnx_socket_t s = accept(
        ls->fd,
        ls->sockaddr,
        &ls->socklen
    );
    if (!bnx_isvalid_socket(s)) {
        int en = bnx_get_socket_errno();
        fprintf(stderr, "[error] accept() failed: (%d)\n", en);
        char *s = (char *)calloc(50, sizeof(char));
        sprintf(s,"accept() failed (%d)\n", en);
        bnx_string_t ls = bnx_create_string(s);
        logger->fwriter(ls, logger);
        free(s);
    }
    return s;
}