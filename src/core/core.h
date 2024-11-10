#ifndef CORE_H
#define CORE_H

#define BNX_OK 0
#define BNX_NG 1

typedef int bnx_int_t;
typedef unsigned int bnx_uint_t;
typedef unsigned char u_char;
typedef struct bnx_listening_s bnx_listening_t;
typedef struct bnx_logger_s bnx_logger_t;

#include "./bnx_conf_setting.h"
#ifdef BNX_WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#endif /** ifdef BNX_WIN32 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>
#include <sys/types.h>

#ifdef BNX_WIN32
#include "../os/win32/bnx_process.h"
#else
#include "../os/unix/bnx_process.h"
#endif /** ifdef BNX_WIN32 */

#include "../os/bnx_socket.h"
#include "../os/unix/bnx_linux_config.h"

#include "./bnx_string.h"
#include "./bnx_log.h"
#include "./bnx_conf_file.h"
#include "./bnx_connection.h"
#include "./bnx_inet.h"

#endif /*CORE_H*/