#include <stdio.h>

//Branch prediction
#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)


extern void noreturn_func() __attribute__((noreturn));

void noreturn_func()
{
    printf("I am a noreturn func \n");
}

void inline_func()
{
    printf("I am a inline func \n");
}
