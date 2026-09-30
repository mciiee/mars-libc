#include "stddef.h"
#include "stdint.h"
#include "syscall.h"

#include "malloc.h"



static MallocMeta _malloc_meta = {0};

static FreeBlockList head = {0};

static inline void malloc_warmup(size_t initSize) {
  _malloc_meta.segmentSize = initSize;
  head.size = initSize;
}

void _malloc_init(void) {
  _malloc_meta.dataSegmentPtr = sbrk(0);
  _malloc_meta.segmentSize = 0;
}

static inline FreeBlockList *findFreeBlock(FreeBlockList *list, size_t chunksize) {
  while (list != nullptr) {
    if ((list->size % sizeof(max_align_t)) >= chunksize) {
      return list;
    }
    list = list->next;
  }

  return nullptr;
}

void *malloc(size_t size) {
  if (_malloc_meta.segmentSize == 0) {
    malloc_warmup(((size/MALLOC_INIT_ALLOC_SIZE) + 1) * MALLOC_INIT_ALLOC_SIZE);
  }

  FreeBlockList *block = findFreeBlock(&head, size);
  if (block == nullptr) {
    return nullptr;
  }
  void *ret = _malloc_meta.dataSegmentPtr + block->startOffset;

  block->size - size;
}
