#undef syscall
#include <stdarg.h>
#include "syscall.h"
#include "stdint.h"


// $2,$3 : $v0,$v1
// $4-$7 : $a0-$a3
// $16-$23 : $s0-$s7

// Reserved for future (mis-)use
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
  int ret = syscall1(9, num);
  return (void*)ret;
}
