#include "stdlib.h"
#include "syscall.h"

[[noreturn]]
void abort(void) {
  _syscall_abort();
  __builtin_unreachable();
}

[[noreturn]]
void exit(int code){
  _syscall_exit(code);
  __builtin_unreachable();
}
