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
    SYS_PRINT_INT_HEX   = 34,
    SYS_PRINT_INT_BINARY = 35,
    SYS_PRINT_INT_UNSINGED = 36
};

int syscall(enum SyscallNumber n, ...);

// For internal (mis-)use
#ifdef MARS_LIBC_IMPLEMENTATION
_Noreturn void _syscall_exit(int code);
_Noreturn void _syscall_abort(void);
void* _syscall_sbrk(size_t num);
#endif

// Almost verbatim MARS syscalls wrappers
#ifdef MARS_LIBC_SYSCALL_WRAPPERS_ENABLE
void mars_print_int(int input);
void mars_print_string(const char *str);
int mars_read_int(void);
void mars_read_string(char *buffer, int bufferSize);
void *mars_sbrk(size_t num);
_Noreturn void mars_exit(void);
void mars_print_char(char c);
char mars_read_char(void);
_Noreturn void mars_exit2(int code);
void mars_print_int_hex(int input);
void mars_print_int_bin(int input);
void mars_print_int_unsigned(int input);
#endif
#ifdef MARS_LIBC_GENERAL_SYSCALL_WRAPPER_ENABLE
void *sbrk(size_t size);
#endif
#endif
