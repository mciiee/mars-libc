#ifndef MARS_LIBC_MALLOC_H
#define MARS_LIBC_MALLOC_H

#include "stddef.h"

#ifndef MALLOC_INIT_ALLOC_SIZE
#define MALLOC_INIT_ALLOC_SIZE 1024 * 1024 // 1 KB
#endif

#ifndef MARS_HEAP_SIZE_LIMIT 
#define MARS_HEAP_SIZE_LIMIT 4 * 1024 * 1024 * 1024 // 4 MB
#endif

typedef struct MallocMeta {
  void *dataSegmentPtr;
  unsigned int segmentSize;
} MallocMeta;

// Idea taken from [https://dmitrysoshnikov.com/compilers/writing-a-memory-allocator/]
typedef struct Block {
  _Bool unused;
  unsigned int size;
  struct Block *next;
} __attribute__((aligned(_Alignof(max_align_t)))) Block;

#if __STDC_VERSION__ >= 201112L
// Max alignment should be 8 bytes
_Static_assert(_Alignof(max_align_t) == 8);
#endif

void _malloc_init(void);

#endif
