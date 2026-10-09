#ifndef _MARS_LIBC_STDDEF_H
#define _MARS_LIBC_STDDEF_H

#ifndef NULL
#if __STDC_VERSION__ >= 202311L
#define NULL nullptr
#else 
#define NULL (void*)0
#endif
#endif

#define offsetof(type, member) __builtin_offsetof(type, member)

typedef __SIZE_TYPE__ size_t;
typedef __PTRDIFF_TYPE__ ptrdiff_t;

typedef unsigned long long max_align_t;
#endif
