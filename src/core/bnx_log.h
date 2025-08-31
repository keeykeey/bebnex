#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#define BNX_LOG_LEVEL_FATAL   1
#define BNX_LOG_LEVEL_ERROR   2
#define BNX_LOG_LEVEL_INFO    3
#define BNX_LOG_LEVEL_DEBUG   4


#define BNX_DEBUG_LOG_PREFIX       "[%s:%d:%s] " 
#define BNX_DEBUG_LOG_ARGS         __FILE__, __LINE__, __func__


#ifndef BNX_LOG_LEVEL
#define BNX_LOG_LEVEL               BNX_LOG_LEVEL_INFO
#endif


#if BNX_LOG_LEVEL >= BNX_LOG_LEVEL_FATAL
    #ifdef BNX_DEBUG
    #define  BNX_LOG_FATAL(fmt, ...) fprintf(stderr, "[FATAL] "BNX_DEBUG_LOG_PREFIX fmt "\n", BNX_DEBUG_LOG_ARGS, ##__VA_ARGS__)
    #else
    #define  BNX_LOG_FATAL(fmt, ...) fprintf(stderr, "[FATAL] " fmt"\n", ##__VA_ARGS__)
    #endif /** BNX_DEBUG */
#else
#define BNX_LOG_FATAL(fmt, ...)
#endif /** BNX_LOG_LEVEL >= BNX_LOG_LEVEL_FATAL */


#if BNX_LOG_LEVEL >= BNX_LOG_LEVEL_ERROR
    #ifdef BNX_DEBUG
    #define  BNX_LOG_ERROR(fmt, ...) fprintf(stderr, "[ERROR] "BNX_DEBUG_LOG_PREFIX fmt "\n", BNX_DEBUG_LOG_ARGS, ##__VA_ARGS__)
    #else
    #define  BNX_LOG_ERROR(fmt, ...) fprintf(stderr, "[ERROR] " fmt"\n", ##__VA_ARGS__)
    #endif /** BNX_DEBUG */
#else
#define BNX_LOG_ERROR(fmt, ...)
#endif /** BNX_LOG_LEVEL >= BNX_LOG_LEVEL_ERROR */


#if BNX_LOG_LEVEL >= BNX_LOG_LEVEL_INFO
    #ifdef BNX_DEBUG
    #define  BNX_LOG_INFO(fmt, ...) fprintf(stdout, "[INFO] "BNX_DEBUG_LOG_PREFIX fmt "\n", BNX_DEBUG_LOG_ARGS, ##__VA_ARGS__)
    #else
    #define  BNX_LOG_INFO(fmt, ...) fprintf(stdout, "[INFO] " fmt"\n", ##__VA_ARGS__)
    #endif /** BNX_DEBUG */
#else
#define BNX_LOG_INFO(fmt, ...)
#endif /** BNX_LOG_LEVEL >= BNX_KOG_LEVEL_INFO */


#if BNX_LOG_LEVEL >= BNX_LOG_LEVEL_DEBUG
    #ifdef BNX_DEBUG
    #define  BNX_LOG_DEBUG(fmt, ...) fprintf(stdout, "[DEBUG] " BNX_DEBUG_LOG_PREFIX fmt "\n", BNX_DEBUG_LOG_ARGS, ##__VA_ARGS__)
    #else
    #define BNX_LOG_DEBUG(fmt, ...) fprintf(stdout, "[DEBUG] "fmt"\n", ##__VA_ARGS__)
    #endif /** BNX_DEBUG */
#else
#define BNX_LOG_DEBUG(fmt, ...)
#endif /** BNX_LOG_LEVEL >= BNX_LOG_LEVEL_DEBUG */

#ifdef __cplusplus
}
#endif