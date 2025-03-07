/* include/mlc/config.h */

#ifndef MLC_CONFIG_H
#define MLC_CONFIG_H

#include <stdio.h>

/* 
 * Define MLC_DEBUGGER before including any MLC header to enable debug messages.
 * Example:
 *   #define MLC_DEBUGGER
 *   #include <mlc/activations.h>
 * If MLC_DEBUGGER is not defined, debug messages will be disabled.
 */
#ifdef MLC_DEBUGGER
    #define LOG_ERROR(msg) fprintf(stderr, "Error: %s\n", msg)
#else
    #define LOG_ERROR(msg) do {} while (0)
#endif

#endif /* MLC_CONFIG_H */