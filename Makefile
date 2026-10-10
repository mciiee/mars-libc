CC=clang
AR=llvm-ar

BUILD_DIR=build
TEST_DIR=test

WFLAGS=-Wall -Wconversion -Wshadow
DFLAGS=-DMARS_LIBC_IMPLEMENTATION -DMARS_LIBC_GENERAL_SYSCALL_WRAPPER_ENABLE -DMARS_LIBC_SYSCALL_WRAPPERS_ENABLE
CFLAGS=-std=c23 $(WFLAGS) $(DFLAGS)
CLANG_FLAGS=-target mips -mllvm -disable-mips-delay-filler -mno-abicalls -G 0
IFLAGS=-Isrc/stdlib -Isrc/syscall -I src/string


.PHONY: all



all: $(BUILD_DIR)/libc.a $(BUILD_DIR)/syscall.asm $(BUILD_DIR)/stdlib.asm $(BUILD_DIR)/assert.asm $(BUILD_DIR)/string.asm $(BUILD_DIR)/malloc.asm 

$(BUILD_DIR)/syscall.asm: src/syscall/syscall.c src/syscall/syscall.h
	$(CC) -S $(CFLAGS) $(CLANG_FLAGS) $(IFLAGS) $< -o $@

$(BUILD_DIR)/stdlib.asm: src/stdlib/stdlib.c src/stdlib/stdlib.h 
	$(CC) -S $(CFLAGS) $(CLANG_FLAGS) $(IFLAGS) $< -o $@

$(BUILD_DIR)/malloc.asm: src/stdlib/malloc.c src/stdlib/malloc.h
	$(CC) -S $(CFLAGS) $(CLANG_FLAGS) $(IFLAGS) $< -o $@

$(BUILD_DIR)/string.asm: src/string/string.c src/string/string.h
	$(CC) -S $(CFLAGS) $(CLANG_FLAGS) $(IFLAGS) $< -o $@

$(BUILD_DIR)/assert.asm: src/assert/assert.c src/assert/assert.h
	$(CC) -S $(CFLAGS) $(CLANG_FLAGS) $(IFLAGS) $< -o $@


$(BUILD_DIR)/libc.a: $(BUILD_DIR)/syscall.o $(BUILD_DIR)/stdlib.o $(BUILD_DIR)/assert.o $(BUILD_DIR)/string.o $(BUILD_DIR)/malloc.o
	$(AR) -crs $@ $^

$(BUILD_DIR)/syscall.o: src/syscall/syscall.c src/syscall/syscall.h
	$(CC) -c $(CFLAGS) $(CLANG_FLAGS) $(IFLAGS) $< -o $@

$(BUILD_DIR)/stdlib.o: src/stdlib/stdlib.c src/stdlib/stdlib.h 
	$(CC) -c $(CFLAGS) $(CLANG_FLAGS) $(IFLAGS) $< -o $@

$(BUILD_DIR)/malloc.o: src/stdlib/malloc.c src/stdlib/malloc.h
	$(CC) -c $(CFLAGS) $(CLANG_FLAGS) $(IFLAGS) $< -o $@

$(BUILD_DIR)/string.o: src/string/string.c src/string/string.h
	$(CC) -c $(CFLAGS) $(CLANG_FLAGS) $(IFLAGS) $< -o $@

$(BUILD_DIR)/assert.o: src/assert/assert.c src/assert/assert.h
	$(CC) -c $(CFLAGS) $(CLANG_FLAGS) $(IFLAGS) $< -o $@

# $(BUILD_DIR)/test: $(TEST_DIR)/test.c $(BUILD_DIR)/libc.a
# 	$(CC) $(CFLAGS) -nostdlib $(CLANG_FLAGS) $(IFLAGS) $(BUILD_DIR)/libc.a $< -o $@

clean: 
	rm -rf $(BUILD_DIR)/*
