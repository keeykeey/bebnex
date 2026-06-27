#pragma once

#ifdef __cplusplus__
extern "C" {
#endif

#define bnx_success(code) ((bnx_error_t){code, NULL, NULL, 0})              // BNX_OK or BNX_DONE is expected
#define bnx_error(code, msg) ((bnx_error_t){code, msg, __FILE__, __LINE__})

typedef enum bnx_code_e {
    BNX_OK       =  0,
    BNX_ERROR    = -1,
    BNX_AGAIN    = -2,
    BNX_DECLINED = -3,
    BNX_BUSY     = -4,
    BNX_DONE     = -5,
    BNX_ABORT    = -6,
} bnx_code_t;

typedef struct bnx_error_s {
    bnx_code_t  code;
    const char *message;
    const char *file;    // __FILE__
    int line;            // __LINE__
} bnx_error_t;

#ifdef __cplusplus__
}
#endif
