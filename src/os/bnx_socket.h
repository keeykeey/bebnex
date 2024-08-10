#ifndef BNX_SOCKET_H
#define BNX_SOCKET_H

#include "../core/config.h"
#include "../core/core.h"


/* if os is windows or not */
#ifdef _WIN32
typedef SOCKET bnx_socket_t;
#define bnx_isvalid_socket(s) ((s) != INVALID_SOCKET)
#define bnx_close_socket(s) closesocket(s)
#define bnx_get_socket_errno() (WSAGetLastError())
#else
typedef int bnx_socket_t;
#define bnx_isvalid_socket(s) ((s) >= 0)
#define bnx_close_socket(s) close(s)
#define bnx_get_socket_errno() (errno)
#endif /** BNX_WIN32 */


bnx_socket_t bnx_socket(int __domain, int __Type, int __protocol, bnx_logger_t *logger);
bnx_socket_t bnx_accept(bnx_listening_t *ls, bnx_logger_t *logger);


#endif /** BNX_SOCKET_H */