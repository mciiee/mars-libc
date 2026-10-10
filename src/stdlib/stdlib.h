#ifndef _MARS_LIBC_STDLIB_H
#define _MARS_LIBC_STDLIB_H

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

#include "stddef.h"

void abort(void);
_Noreturn void exit(int code);
int atexit(void (*function)(void));
_Noreturn void quick_exit(int code);
int at_quick_exit(void (*func)(void));
void* malloc(size_t size);
void free(void* ptr);

#ifdef MARS_LIBC_IMPLEMENTATION
void _libc_stdlib_init(void);
#include "malloc.h"
#endif
#endif
