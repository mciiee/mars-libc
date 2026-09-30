#ifndef _LIBC_MALLOC_H
#define _LIBC_MALLOC_H

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

typedef struct FreeBlockList {
  unsigned int startOffset;
  unsigned int size;
  struct FreeBlockList *next;
} FreeBlockList;


void _malloc_init(void);

#endif
