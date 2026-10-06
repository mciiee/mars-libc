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
  constexpr unsigned int funcNum = sizeof(_atExitFuncs)/sizeof(_atExitFuncs[0]);
  for (unsigned int i = 1; i <= funcNum; i++) {
    if (_atExitFuncs[funcNum - i] != nullptr) {
      _atExitFuncs[funcNum - i]();
    }
  }
  _syscall_exit(code);
  __builtin_unreachable();
}

int atexit(void (*function)(void)) {
  constexpr unsigned int funcNum = sizeof(_atExitFuncs)/sizeof(_atExitFuncs[0]);
  for (unsigned int i = 0; i < funcNum; i++) {
    if (_atExitFuncs[i] == nullptr) {
      _atExitFuncs[i] = function;
      return 0;
    }
  }

  return -1;
}

int at_quick_exit(void (*function)(void)) {
  constexpr unsigned int funcNum = sizeof(_atQuickExitFuncs)/sizeof(_atQuickExitFuncs[0]);
  for (unsigned int i = 0; i < funcNum; i++) {
    if (_atQuickExitFuncs[i] == nullptr) {
      _atQuickExitFuncs[i] = function;
      return 0;
    }
  }

  return -1;
}


[[noreturn]]
void quick_exit(int code) {
  constexpr unsigned int funcNum = sizeof(_atQuickExitFuncs)/sizeof(_atQuickExitFuncs[0]);
  for (unsigned int i = 1; i <= funcNum; i++) {
    if (_atQuickExitFuncs[funcNum - i] != nullptr) {
      _atQuickExitFuncs[funcNum - i]();
    }
  }
  _syscall_exit(code);
  __builtin_unreachable();
}


inline void _libc_stdlib_init(void) {
  _malloc_init();
}
