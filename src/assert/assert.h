#ifndef _MARS_LIBC_ASSERT_H
#define _MARS_LIBC_ASSERT_H

#ifdef NDEBUG
#define assert(expr) ((void)(0))
#else
#define assert(expr) do { if(!(expr)) { _mars_assert_fail(#expr, __FILE__, __LINE__); } } while(0)
#endif

_Noreturn void _mars_assert_fail(const char *exprStr, const char *file, int line);
#endif
