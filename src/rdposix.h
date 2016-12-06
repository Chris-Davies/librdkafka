#pragma once
/*
* librdkafka - Apache Kafka C library
*
* Copyright (c) 2012-2015 Magnus Edenhill
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* 1. Redistributions of source code must retain the above copyright notice,
*    this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright notice,
*    this list of conditions and the following disclaimer in the documentation
*    and/or other materials provided with the distribution.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*/

/**
 * POSIX system support
 */
#pragma once

#include <unistd.h>
#include <sys/time.h>
#include <inttypes.h>

/**
* Types
*/


/**
 * Annotations, attributes, optimizers
 */
#ifndef likely
#define likely(x)   __builtin_expect((x),1)
#endif
#ifndef unlikely
#define unlikely(x) __builtin_expect((x),0)
#endif

#define RD_UNUSED   __attribute__((unused))
#define RD_INLINE   inline
#define RD_WARN_UNUSED_RESULT __attribute__((warn_unused_result))
#define RD_NORETURN __attribute__((noreturn))
#define RD_IS_CONSTANT(p)  __builtin_constant_p((p))
#define RD_TLS      __thread

/**
* Allocation
*/
#if !defined(__FreeBSD__)
/* alloca(3) is in stdlib on FreeBSD */
#include <alloca.h>
#endif

#define rd_alloca(N)  alloca(N)


/**
* Strings, formatting, printf, ..
*/

/* size_t and ssize_t format strings */
#define PRIusz  "zu"
#define PRIdsz  "zd"

#define RD_FORMAT(...) __attribute__((format (__VA_ARGS__)))
#define rd_snprintf(...)  snprintf(__VA_ARGS__)
#define rd_vsnprintf(...) vsnprintf(__VA_ARGS__)

#define rd_strcasecmp(A,B) strcasecmp(A,B)
#define rd_strncasecmp(A,B,N) strncasecmp(A,B,N)

/**
 * Errors
 */
#define rd_strerror(err) strerror(err)


/**
 * Atomics
 */
#include "rdatomic.h"

/**
* Misc
*/

/**
 * Microsecond sleep.
 * Will retry on signal interrupt unless *terminate is true.
 */
static RD_INLINE RD_UNUSED
void rd_usleep (int usec, rd_atomic32_t *terminate) {
        struct timespec req = {usec / 1000000, (long)(usec % 1000000) * 1000};

        /* Retry until complete (issue #272), unless terminating. */
        while (nanosleep(&req, &req) == -1 &&
               (errno == EINTR && (!terminate || !rd_atomic32_get(terminate))))
                ;
}


#if !HAVE_QSORT_R
void qsort_r (void *base, size_t nmemb, size_t size,
              int (*compar)(const void *, const void *, void *),
              void *arg);
#endif


#define rd_gettimeofday(tv,tz)  gettimeofday(tv,tz)


#define rd_assert(EXPR)  assert(EXPR)

/**
 * Empty struct initializer
 */
#define RD_ZERO_INIT  {}
