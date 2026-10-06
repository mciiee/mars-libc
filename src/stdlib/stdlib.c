#include "stdlib.h"
#include "syscall.h"
#include "malloc.h"

static void (*_atExitFuncs[32])(void) = {nullptr};
static void (*_atQuickExitFuncs[32])(void) = {nullptr};

[[noreturn]]
void abort(void) {
  _syscall_abort();
  __builtin_unreachable();
}

[[noreturn]]
void exit(int code){
  for (unsigned int i = 0; i < sizeof(_atExitFuncs)/sizeof(_atExitFuncs[0]); i++) {
    if (_atExitFuncs[i] != nullptr) {
      _atExitFuncs[i]();
    }
  }
  _syscall_exit(code);
  __builtin_unreachable();
}

int atexit(void (*function)(void)) {
  for (unsigned int i = 0; i < sizeof(_atExitFuncs)/sizeof(_atExitFuncs[0]); i++) {
    if (_atExitFuncs[i] == nullptr) {
      _atExitFuncs[i] = function;
      return 0;
    }
  }

  return -1;
}

int at_quick_exit(void (*function)(void)) {
  for (unsigned int i = 0; i < sizeof(_atQuickExitFuncs)/sizeof(_atQuickExitFuncs[0]); i++) {
    if (_atQuickExitFuncs[i] == nullptr) {
      _atQuickExitFuncs[i] = function;
      return 0;
    }
  }

  return -1;
}


[[noreturn]]
void quick_exit(int code) {
  for (unsigned int i = 0; i < sizeof(_atQuickExitFuncs)/sizeof(_atQuickExitFuncs[0]); i++) {
    if (_atQuickExitFuncs[i] != nullptr) {
      _atQuickExitFuncs[i]();
    }
  }
  _syscall_exit(code);
  __builtin_unreachable();
}


inline void _libc_stdlib_init(void) {
  _malloc_init();
}
