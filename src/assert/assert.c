#include "assert.h"
#include "syscall.h"


[[noreturn]]
void _mars_assert_fail(const char *exprStr, const char *file, int line) {
  mars_print_char('[');
  mars_print_string(file);
  mars_print_char(':');
  mars_print_int(line);
  mars_print_char(']');
  mars_print_char(' ');
  mars_print_string("Assertion failed: (");
  mars_print_string(exprStr);
  mars_print_char(')');
  mars_print_char('\n');

  mars_exit();
}

