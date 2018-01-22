/*
 * koeva-util-debug.h: utility function calls for debugging
 *
 * BSD 3-clause license
 * Author : Al Rayan, Bogor Agricultural University, 2018 <alrayan12t@apps.ipb.ac.id>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the BSD 3-Clause License as 
 * published by the BSD Organization; 3-Clause version of the
 * license.
 */

#ifndef KOEVA_UTIL_DEBUG__H
#define KOEVA_UTIL_DEBUG__H

#include <stdio.h>
#include <errno.h>

#    ifdef KV_VERBOSE

#        ifdef KV_VERBOSE_WFORMAT

/*
 * Format escape character for unix terminal
 */
#define KV_ERF "\e[101m"
#define KV_BOF "\e[1m"
#define KV_NOF "\e[0m"
#define KV_WAF "\e[43m"

#        else

/*
 * Empty
 */
#define KV_ERF
#define KV_BOF
#define KV_NOF
#define KV_WAF

#        endif

#        ifdef KV_DEBUG

/*
 * Logging debug message to standard error
 */
#define LOG_D(msg, ...) \
        fprintf(stderr, "DEBUG @%s:%d :" msg "\n", \
                __FILE__, __LINE__, ##__VA_ARGS__ )

#        else

/*
 * Empty
 */
#define LOG_D

#        endif

/*
 * Logging error to standard error
 */
#define LOG_E(msg, ...) \
        fprintf(stderr, KV_ERF "ERROR @%s:%d :" KV_NOF  msg "\n", \
                __FILE__, __LINE__, ##__VA_ARGS__ )
                
/*
 * Logging info to standard error
 */
#define LOG_I(msg, ...) \
        fprintf(stderr, KV_BOF "INFO @%s:%d :" KV_NOF msg "\n", \
                __FILE__, __LINE__, ##__VA_ARGS__ )

/*
 * Logging warning to standard error
 */
#define LOG_W(msg, ...) \
        fprintf(stderr, KV_WAF "WARNING @%s:%d :" KV_NOF msg "\n", \
                __FILE__, __LINE__, ##__VA_ARGS__ )

#    else

/*
 * Empty
 */
#define LOG_I
#define LOG_W
#define LOG_E

#    endif

/*
 * Assertion for debug, jump if error occur
 */
#define ASSERT_DEBUG(assert, msg, ...) \
        if(!(assert)) { LOG_D(msg, ##__VA_ARGS__); \
        errno = 0; goto error; }

/*
 * Assertion for normal mode, print error and jump if error occur
 */
#define ASSERT(assert, msg, ...) \
        if(!(assert)) { LOG_E(msg, ##__VA_ARGS__); \
        errno = 0; goto error; }

/*
 * Assertion for memory, print error and jump if memory is null
 */
#define ASSERT_MEM(assert) \
        ASSERT((assert), "Memory couldn't be allocated.")

/*
 * Error propagation functor, print error and jump
 */
#define KV_ERROR(msg, ...) \
        { LOG_E(msg, ##__VA_ARGS__); \
        errno = 0; goto error; }

#endif
