#ifndef _BASE_MACROS_H_
#define _BASE_MACROS_H_

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
# define BASE_CPP_START extern "C" {
# define BASE_CPP_END }
#else
# define BASE_CPP_START
# define BASE_CPP_END
#endif

#define likely(x)                   __builtin_expect(!!(x), 1)
#define unlikely(x)                 __builtin_expect(!!(x), 0)

#define base_free(ptr)              if(ptr) free(ptr)
#define base_malloc(size)           malloc(size)
#define easy_realloc(ptr, size)     realloc(ptr, size)
#define base_align_ptr(p, a)        (uint8_t*)(((uintptr_t)(p) + ((uintptr_t) a - 1)) & ~((uintptr_t) a - 1))
#define base_align(d, a)            (((d) + (a - 1)) & ~(a - 1))
#define base_max(a,b)               (a > b ? a : b)
#define base_min(a,b)               (a < b ? a : b)
#define base_div(a,b)               ((b) ? ((a)/(b)) : 0)
#define base_memcpy(dst, src, n)    (((char *) memcpy(dst, src, (n))) + (n))
#define base_const_strcpy(b, s)     easy_memcpy(b, s, sizeof(s)-1)
#define base_safe_close(fd)         {if((fd)>=0){close((fd));(fd)=-1;}}
#define base_ignore(exp)            {int ignore __attribute__ ((unused)) = (exp);}

#endif // end of _BASE_MACROS_H_H
