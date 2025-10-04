#pragma once

#ifdef __cplusplus__
extern "C" {
#endif

typedef enum bnx_code {
    BNX_OK = 0,
    BNX_ERROR = -1,
    BNX_STOP = -2,
    BNX_CONTINUE = -3,
    BNX_INVALID_ARGUMENT = -4,
    BNX_MEMORY_ERROR = -5,
    BNX_EOF = -6,
} bnx_code_e;

#ifdef __cplusplus__
}
#endif