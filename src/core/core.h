#ifndef CORE_H
#define CORE_H

#define BNX_OK 0
#define BNX_NG 1

typedef int bnx_int_t;
typedef unsigned int bnx_uint_t;
typedef unsigned char u_char;

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>

#include "../os/unix/bnx_socket.h"
#include "../os/unix/bnx_linux_config.h"

#include "./bnx_string.h"
#include "./bnx_log.h"
#include "./bnx_conf_file.h"
#include "./bnx_connection.h"
#include "./bnx_inet.h"


#endif /*CORE_H*/