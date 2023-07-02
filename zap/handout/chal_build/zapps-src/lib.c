// gcc -o ./ld-linux-x86-64.so.2 ./lib.c -fPIC -nostdlib
#include <elf.h>
#include <fcntl.h>
#include <limits.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/mman.h>
#include <sys/syscall.h>
#include <unistd.h>

#define STDERR_FILENO 2

long _zapps_syscall(long n, ...)
{
    long a, b, c, d, e, f;
    unsigned long ret;
    va_list ap;

    va_start(ap, n);
    a = va_arg(ap, long);
    b = va_arg(ap, long);
    c = va_arg(ap, long);
    d = va_arg(ap, long);
    e = va_arg(ap, long);
    f = va_arg(ap, long);
    va_end(ap);

    register long r10 __asm__("r10") = d;
    register long r8 __asm__("r8") = e;
    register long r9 __asm__("r9") = f;
    __asm__ __volatile__ (
        "syscall" :
        "=a"(ret) :
        "a"(n), "D"(a), "S"(b), "d"(c), "r"(r10), "r"(r8), "r"(r9) :
        "rcx", "r11", "memory"
    );
    return ret;
}

int _zapps_sys_open(const char *pathname, int flags)
{
    return _zapps_syscall(SYS_open, pathname, flags);
}
static inline 
int _zapps_sys_close(int fd)
{
    return _zapps_syscall(SYS_close, fd);
}
static inline 
ssize_t _zapps_sys_read(int fd, void *buf, size_t count)
{
    return _zapps_syscall(SYS_read, fd, buf, count);
}
static inline 
ssize_t _zapps_sys_write(int fd, const void *buf, size_t count)
{
    return _zapps_syscall(SYS_write, fd, buf, count);
}

int _start()
{
    // char* argv[] = { "/bin/sh", NULL };
    // char* envp[] = { NULL };
    // _zapps_syscall(SYS_execve, "/bin/sh", argv, envp);

    int flag_in = _zapps_sys_open("/mnt/flag", O_RDONLY);
    char flag[256] = {0};
    int rc;

    rc = _zapps_sys_read(flag_in, flag, 256);
    flag[rc] = '\n';

    _zapps_sys_write(STDERR_FILENO, flag, rc);
}
