#include "../core/core.h"
#include "../core/config.h"

bnx_socket_t bnx_socket(int __domain, int __type, int __protocol, bnx_logger_t *errlogger)
{
    bnx_socket_t fd = socket(__domain, __type, __protocol);
    if (!bnx_isvalid_socket(fd)) {
        int eno = bnx_get_socket_errno();
        errlogger->fwriter(bnx_vsnerrorlog(30, "socket() failed (%d)\n", eno), errlogger);
    }
    return fd;

}

bnx_socket_t bnx_accept(bnx_listening_t *ls, bnx_logger_t *errlogger)
{
    bnx_socket_t s = accept(
        ls->fd,
        ls->sockaddr,
        &ls->socklen
    );
    if (!bnx_isvalid_socket(s)) {
        int en = bnx_get_socket_errno();
        errlogger->fwriter(bnx_vsnerrorlog(30, "accept() failed (%d)\n"), errlogger);
    }
    return s;
}