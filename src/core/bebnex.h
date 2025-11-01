#pragma once

#ifdef __cplusplus__
extern "C" {
#endif

typedef enum bnx_code {
    BNX_OK = 0,
    BNX_ERROR = -1,
    BNX_RUNTIME_ERROR = -2,
    BNX_STOP = -3,
    BNX_CONTINUE = -4,
    BNX_INVALID_ARGUMENT = -5,
    BNX_MEMORY_ERROR = -6,
    BNX_EOF = -7,
} bnx_code_e;

#ifdef __cplusplus__
}
#endif