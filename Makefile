CC=clang

SRC_DIR=src
BUILD_DIR=build

CFLAGS=-std=c23
CLANG_FLAGS=-target mips -mllvm -disable-mips-delay-filler -mno-abicalls -G 0

$(BUILD_DIR)/syscall.o: $(SRC_DIR)/syscall.c $(SRC_DIR)/syscall.h
	clang -c $(CFLAGS) $(CLANG_FLAGS) $< -o $@


$(BUILD_DIR)/stdlib.o: $(SRC_DIR)/stdlib.c $(SRC_DIR)/stdlib.h
	clang -c $(CFLAGS) $(CLANG_FLAGS) $< -o $@

