CC=clang

BUILD_DIR=build

CFLAGS=-std=c23 -D_LIBC_IMPLEMENTATION
CLANG_FLAGS=-target mips -mllvm -disable-mips-delay-filler -mno-abicalls -G 0
IFLAGS=-Isrc/stdlib -Isrc/syscall


.PHONY: all

all: $(BUILD_DIR)/syscall.o $(BUILD_DIR)/stdlib.o

$(BUILD_DIR)/syscall.o: src/syscall/syscall.c src/syscall/syscall.h
	clang -S $(CFLAGS) $(CLANG_FLAGS) $(IFLAGS) $< -o $@

$(BUILD_DIR)/stdlib.o: src/stdlib/stdlib.c src/stdlib/stdlib.h
	clang -S $(CFLAGS) $(CLANG_FLAGS) $(IFLAGS) $< -o $@
