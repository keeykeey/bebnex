#ifndef CORE_H
#define CORE_H

#define BNX_OK 0
#define BNX_NG 1

typedef int bnx_int_t;
typedef unsigned int bnx_uint_t;
typedef struct sockaddr_in sockaddr_in_t;

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "./bnx_string.h"
#include "./bnx_log.h"
#include "./bnx_conf_file.h"
#include "../event/bnx_socket.h"

#endif /*CORE_H*/