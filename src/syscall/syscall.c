#undef syscall
#include <stdarg.h>
#include "syscall.h"
#include "stdint.h"


// $2,$3 : $v0,$v1
// $4-$7 : $a0-$a3
// $16-$23 : $s0-$s7

[[maybe_unused]]
static inline int syscall0(int n) {
    int ret;
    __asm__ volatile(
        "move $v0, %1" "\n"
        "syscall" "\n"
        "move %0, $v0"
        : "=r"(ret)
        : "r"(n)
        : "$1", "$2", "$3", "$4", "$5", "$6", "$7", "$8", "$9", "$10", "$11", "$12", "$13", "$14", "$15", "$24", "$25", "$28", "hi", "lo", "memory"
    );
    return ret;
}

[[maybe_unused]]
static inline int syscall1(int n, int arg) {
  int ret;
  __asm__ volatile(
    "move $v0, %1" "\n"
    "move $a0, %2" "\n"
    "syscall" "\n"
    "move %0, $v0"
    : "=r"(ret)
    : "r"(n), "r"(arg)
    : "$1", "$2", "$3", "$4", "$5", "$6", "$7", "$8", "$9", "$10", "$11", "$12", "$13", "$14", "$15", "$24", "$25", "$28", "hi", "lo", "memory"
  );
  return ret;
}

[[maybe_unused]]
static inline int syscall2(int n, int a0, int a1) {
  int ret;
  __asm__ volatile(
    "move $v0, %1" "\n"
    "move $a0, %2" "\n"
    "move $a1, %3" "\n"
    "syscall" "\n"
    "move %0, $v0"
    : "=r"(ret)
    : "r"(n), "r"(a0), "r"(a1)
    : "$1", "$2", "$3", "$4", "$5", "$6", "$7", "$8", "$9", "$10", "$11", "$12", "$13", "$14", "$15", "$24", "$25", "$28", "hi", "lo", "memory"
  );
  return ret;
}

[[maybe_unused]]
static inline int syscall3(int n, int a0, int a1, int a2) {
  int ret;
  __asm__ volatile(
    "move $v0, %1" "\n"
    "move $a0, %2" "\n"
    "move $a1, %3" "\n"
    "move $a2, %4" "\n"
    "syscall" "\n"
    "move %0, $v0"
    : "=r"(ret)
    : "r"(n), "r"(a0), "r"(a1), "r"(a2)
    : "$1", "$2", "$3", "$4", "$5", "$6", "$7", "$8", "$9", "$10", "$11", "$12", "$13", "$14", "$15", "$24", "$25", "$28", "hi", "lo", "memory"
  );
  return ret;
}

static inline int syscall4(int n, int a0, int a1, int a2, int a3) {
  int ret;
  __asm__ volatile(
    "move $v0, %1" "\n"
    "move $a0, %2" "\n"
    "move $a1, %3" "\n"
    "move $a2, %4" "\n"
    "move $a3, %5" "\n"
    "syscall" "\n"
    "move %0, $v0"
    : "=r"(ret)
    : "r"(n), "r"(a0), "r"(a1), "r"(a2), "r"(a3)
    : "$1", "$2", "$3", "$4", "$5", "$6", "$7", "$8", "$9", "$10", "$11", "$12", "$13", "$14", "$15", "$24", "$25", "$28", "hi", "lo", "memory"
  );
  return ret;
}

[[maybe_unused]]
static unsigned short syscall_arg_num(enum SyscallNumber n) {
  switch (n) {
    case SYS_EXIT:
    case SYS_READ_INT:
    case SYS_READ_FLOAT:
    case SYS_READ_DOUBLE:
    case SYS_READ_CHAR:
      return 0;
    case SYS_PRINT_INT:
    case SYS_PRINT_STRING:
    case SYS_SBRK:
    case SYS_PRINT_CHAR:
    case SYS_CLOSE:
    case SYS_EXIT2:
    case SYS_PRINT_INT_HEX:
    case SYS_PRINT_INT_BINARY:
    case SYS_PRINT_INT_UNSINGED:
      return 1;
    case SYS_READ_STRING:
      return 2;
    case SYS_OPEN:
    case SYS_READ:
    case SYS_WRITE:
      return 3;
    // Floating-point syscalls are not yet implemented
    case SYS_PRINT_FLOAT:
    case SYS_PRINT_DOUBLE:
      break;
    }
  return (unsigned short)(-1);
}

// TODO: Implment floating-point stuff
int syscall(enum SyscallNumber n, ...) {
    va_list ap;
    va_start(ap, n);
    int a0 = va_arg(ap, int);
    int a1 = va_arg(ap, int);
    int a2 = va_arg(ap, int);
    int a3 = va_arg(ap, int);
    va_end(ap);
    return syscall4(n, a0, a1, a2, a3);
}

[[noreturn]]
void _syscall_exit(int code) {
  syscall1(SYS_EXIT2, code);
  __builtin_unreachable();
}

[[noreturn]]
void _syscall_abort(void) {
  syscall0(SYS_EXIT);
  __builtin_unreachable();
}

void* _syscall_sbrk(size_t num) {
  int ret = syscall1(9, (int)num);
  return (void*)ret;
}



void mars_print_int(int input) {
  (void)syscall1(SYS_PRINT_INT, input);
}

void mars_print_string(const char *str) {
  (void)syscall1(SYS_PRINT_STRING, (int)str);
}

int mars_read_int(void) {
  return syscall0(SYS_READ_INT);
}

void mars_read_string(char *buf, int bufSize) {
  (void)syscall2(SYS_READ_STRING, (int)buf, bufSize);
}

// More to be added later(interrupts and stuff)
void *mars_sbrk(size_t num) {
  return _syscall_sbrk(num);
}

[[noreturn]]
void mars_exit(void) {
  syscall0(SYS_EXIT);
  __builtin_unreachable();
}

void mars_print_char(char c) {
  (void)syscall1(SYS_PRINT_CHAR, c);
}

char mars_read_char(void) {
  return (char)syscall0(SYS_READ_CHAR);
}

[[noreturn]]
void mars_exit2(int code) {
  syscall1(SYS_EXIT2, code);
  __builtin_unreachable();
}

void mars_print_int_hex(int input) {
  (void)syscall1(SYS_PRINT_INT_HEX, input);
}
void mars_print_int_bin(int input) {
  (void)syscall1(SYS_PRINT_INT_BINARY, input);
}
void mars_print_int_unsigned(int input) {
  (void)syscall1(SYS_PRINT_INT_UNSINGED, input);
}


// Same as [mars_sbrk]
[[maybe_unused]]
void *sbrk(size_t size) {
  return mars_sbrk(size);
}

