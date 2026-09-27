#ifndef _LIBC_STDLIB_H
#define _LIBC_STDLIB_H

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

#include "stddef.h"

void abort(void);
void exit(int code);
void* malloc(size_t size);
void free(void* ptr);

#endif
