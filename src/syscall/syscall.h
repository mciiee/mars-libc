#ifndef _LIBC_SYSCALL_H
#define _LIBC_SYSCALL_H
#include "stddef.h"

enum SyscallNumber : int {
    SYS_PRINT_INT       = 1,   // print integer (in $a0)
    SYS_PRINT_FLOAT     = 2,   // print float (in $f12)
    SYS_PRINT_DOUBLE    = 3,   // print double (in $f12)
    SYS_PRINT_STRING    = 4,   // print null-terminated string (address in $a0)
    SYS_READ_INT        = 5,   // read integer (result in $v0)
    SYS_READ_FLOAT      = 6,   // read float (result in $f0)
    SYS_READ_DOUBLE     = 7,   // read double (result in $f0)
    SYS_READ_STRING     = 8,   // read string ($a0 = buffer, $a1 = max length)
    SYS_SBRK            = 9,   // allocate heap memory ($a0 = bytes, address in $v0)
    SYS_EXIT            = 10,  // terminate execution
    SYS_PRINT_CHAR      = 11,  // print character (in $a0)
    SYS_READ_CHAR       = 12,  // read character (result in $v0)
    SYS_OPEN            = 13,  // open file ($a0 = filename, $a1 = flags, $a2 = mode)
    SYS_READ            = 14,  // read from file ($a0 = fd, $a1 = buffer, $a2 = count)
    SYS_WRITE           = 15,  // write to file ($a0 = fd, $a1 = buffer, $a2 = count)
    SYS_CLOSE           = 16,  // close file ($a0 = fd)
    SYS_EXIT2           = 17,  // terminate with value (result in $a0)
};

int syscall(enum SyscallNumber n, ...);

#ifdef _LIBC_IMPLEMENTATION
[[noreturn]] void _syscall_exit(int code);
[[noreturn]] void _syscall_abort(void);
void* _syscall_sbrk(size_t num);
#endif
#endif
