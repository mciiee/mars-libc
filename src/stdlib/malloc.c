#include "stddef.h"
#include "stdint.h"
#include "syscall.h"
#include "assert.h"

#include "malloc.h"

constexpr size_t maxAlign = alignof(max_align_t);

static MallocMeta _malloc_meta = {0};


// Assumes that block->next is the closest next block in memory
static Block *start = nullptr;
static Block *top = nullptr;


[[gnu::pure]]
static inline unsigned int alignup(uintptr_t addr, size_t align) {
  return addr + (align - addr % align) % align;
}

[[gnu::pure]]
// The actual size of allocation, including the Block metadata 
static inline size_t allocsize(size_t size, size_t align) {
  size = alignup(size, align);
  size_t blocksize = alignup(sizeof(Block), align);
  return size + blocksize;
}

[[gnu::pure]]
static inline Block *block_header_get(const void *ptr) {
  return ((Block *)ptr)-1;
}

// Allocates a memory chunk with block metadata
// Assumes [size] to include block size
static inline void *block_alloc(size_t size, size_t align) {
  size = alignup(size, align);
  
  void *ret = mars_sbrk(size);
  if (ret == (void*)-1) {
    return nullptr;
  }

  Block *block = ret;
  block->size = size;
  //block->padding = (align - (sizeof(*block) % align)) % align;
  block->unused = true;
  block->next = nullptr;

  _malloc_meta.segmentSize += size;

  return (void*)block;
}

static inline void block_merge(Block *left, Block *right) {
  if (top == right) {
    top = left;
  }
  left->next = right->next;
  left->size += right->size;
}

static inline Block * block_get_previous(const Block *block) {
  if (block == start) {
    return nullptr;
  }

  Block *ret = start;
  while (ret != nullptr && ret->next != block) {
    ret = ret->next;
  }
  return ret;
}


// Assumes that [initSize] is aligned.
// Assumes that [sbrk] has not been called prior with any non-zero argument.
static inline void malloc_warmup(size_t initSize) {
  if (_malloc_meta.segmentSize != 0) {
    return;
  }

  _malloc_meta.dataSegmentPtr = mars_sbrk(initSize);
  if (_malloc_meta.dataSegmentPtr == (void*)-1) {
    mars_print_string("[FATAL]" "malloc_warmup: Call to sbrk(initSize) failed, initSize=");
    mars_print_int((int)initSize);
    mars_exit();
  }

  _malloc_meta.segmentSize = initSize;
  start->size = initSize;
  start->unused = true;
  start->next = nullptr;
  top = start;
}


// Called in libc initiaziation code. Assumed to be called before [malloc_warmup]
void _malloc_init(void) {
  _malloc_meta.dataSegmentPtr = mars_sbrk(0);
  start = _malloc_meta.dataSegmentPtr;

  if (_malloc_meta.dataSegmentPtr == (void*)-1) {
    // THIS SHOULD NOT HAPPEN! Someone fucked up hard
    mars_print_string("[FATAL]" "Failed to initialize malloc: sbrk(0) failed. THIS SHOULD NOT HAPPEN!!!");
    mars_exit();
  }

  // The strictest aligment is 8 bytes. Ensures that the error message is right.
  static_assert(maxAlign == 8);
  if ((uintptr_t)_malloc_meta.dataSegmentPtr % maxAlign != 0) {
    mars_print_string("[FATAL]" "Failed to initialize malloc: heap base address is not 8-byte aligned. THIS SHOULD NOT HAPPEN!!!");
    mars_exit();
  }
  _malloc_meta.segmentSize = 0;
}

// [align] is here for future implementation of [aligned_alloc]
static inline bool chunkFitsAfterAlignment(Block *block, size_t chunksize, size_t align) {
  if (chunksize > block->size) {
    return false;
  }

  const uintptr_t offset = alignup((uintptr_t)block, align);
  chunksize = alignup(chunksize, align);
  
  // The block is too small
  if (offset + chunksize > (uintptr_t)block + block->size) {
    return false;
  }

  return true;
}

// [align] is here for future implementation of [aligned_alloc]
static inline Block *findFreeBlock(Block *list, size_t chunksize, size_t align) {
  while (list != nullptr) {
    if (chunkFitsAfterAlignment(list, chunksize, align) && list->unused) {
      return list;
    }
    list = list->next;
  }
  return nullptr;
}


// Does not support [align] > [maxAlign] (8)
static void *alloc_weak_align(size_t align, size_t size) {
  assert(start != nullptr);
  assert(align <= maxAlign);

  if (_malloc_meta.segmentSize == 0) {
    malloc_warmup(((size/MALLOC_INIT_ALLOC_SIZE) + 1) * MALLOC_INIT_ALLOC_SIZE);
  }

  size_t allocSize = allocsize(size, align);

  Block *block = findFreeBlock(start, allocSize, align);
  if (block == nullptr) {
    Block *newBlock = block_alloc(allocSize, align);
    if (newBlock == nullptr) {
      return nullptr;
    }
    top->next = newBlock;
    top = newBlock;
    block = newBlock;
  }
  // Split if block size is big _enough_
  // _enough_ means that it can house at least 1 [align]-aligned block
  else if (block->size >= allocSize + sizeof(Block) + align){
    Block *blk = (Block *)(((uintptr_t)block) + allocSize);
    blk->size = block->size - allocSize;
    blk->next = block->next;
    blk->unused = true;
    
    block->size = allocSize;
    block->next = blk;
    if (block == top) {
      top = blk;
    }
  }

  block->unused = false;
  return (void*)(block + 1);
}

void *malloc(size_t size) {
  if (size == 0) {
    return nullptr;
  }
  return alloc_weak_align(maxAlign, size);
}

void free(void *ptr) {
  if (ptr == nullptr) {
    return;
  }

  Block *header = block_header_get(ptr);
  if (header->unused) {
    mars_print_string("[Error]" "Double free at address: ");
    mars_print_int_hex((int)ptr);
    mars_exit();
  }

  header->unused = true;
  if (header->next != nullptr && header->next->unused) {
    block_merge(header, header->next);
  }

  Block *prev = block_get_previous(header);
  if (prev != nullptr && prev->unused) {
    block_merge(prev, header);
  }
}
