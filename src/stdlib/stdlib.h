#ifndef _LIBC_STDLIB_H
#define _LIBC_STDLIB_H

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

#include "stddef.h"

void abort(void);
void exit(int code);
int atexit(void (*function)(void));
void* malloc(size_t size);
void free(void* ptr);

#ifdef _LIBC_IMPLEMENTATION
#include "malloc.h"
#endif
#endif
