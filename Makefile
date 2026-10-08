CC=clang

BUILD_DIR=build

CFLAGS=-std=c23 -D_LIBC_IMPLEMENTATION -D_LIBC_SYSCALL_WRAPPERS_ENABLE -D_LIBC_GENERAL_SYSCALL_WRAPPER_ENABLE
CLANG_FLAGS=-target mips -mllvm -disable-mips-delay-filler -mno-abicalls -G 0
IFLAGS=-Isrc/stdlib -Isrc/syscall


.PHONY: all

all: $(BUILD_DIR)/syscall.asm $(BUILD_DIR)/stdlib.asm $(BUILD_DIR)/assert.asm # $(BUILD_DIR)/malloc.asm $(BUILD_DIR)/string.asm

$(BUILD_DIR)/syscall.asm: src/syscall/syscall.c src/syscall/syscall.h
	clang -S $(CFLAGS) $(CLANG_FLAGS) $(IFLAGS) $< -o $@

$(BUILD_DIR)/stdlib.asm: src/stdlib/stdlib.c src/stdlib/stdlib.h 
	clang -S $(CFLAGS) $(CLANG_FLAGS) $(IFLAGS) $< -o $@

$(BUILD_DIR)/malloc.asm: src/stdlib/malloc.c src/stdlib/malloc.h
	clang -S $(CFLAGS) $(CLANG_FLAGS) $(IFLAGS) $< -o $@

$(BUILD_DIR)/string.asm: src/string/string.c src/string/string.h
	clang -S $(CFLAGS) $(CLANG_FLAGS) $(IFLAGS) $< -o $@

$(BUILD_DIR)/assert.asm: src/assert/assert.c src/assert/assert.h
	clang -S $(CFLAGS) $(CLANG_FLAGS) $(IFLAGS) $< -o $@



clean: 
	rm -rf $(BUILD_DIR)/*
