# Assembly

## system calls

system calls have very well-defined interfaces that very rarely change.There are over 300 system calls in linux.

`int open(const char *pathname, int flags)` - return a file new file descriptor of the open file.
`ssize_t read(int fd, void *buf, size_t count)` - read data from the file descriptor.
`pid_t fork()` - forks off an identical child process.
`int execve(const char *filename, char **argv,char **envp)` - replaces your process.
`pid_t wait(int *wstatus)` - wait child termination, return its PID, write its status into *wstatus.

> code

```s
.globl _start
_start:
.intel_syntax noprefix
  mov rax, 60
  mov rdi, 42
  syscall

```

> compile

`gcc -nostdlib -o exit exit.s` or `gcc -static -nostdlib -o exit exit.s`

> strace output

```bash
strace ./exit

execve("./exit", ["./exit"], 0x7fff79ac8350 /* 54 vars */) = 0
brk(NULL)                               = 0x55f34d4c0000
arch_prctl(0x3001 /* ARCH_??? */, 0x7fff294da310) = -1 EINVAL (Invalid argument)
access("/etc/ld.so.preload", R_OK)      = -1 ENOENT (No such file or directory)
mmap(NULL, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7f7d7eaff000
arch_prctl(ARCH_SET_FS, 0x7f7d7eaffa80) = 0
set_tid_address(0x7f7d7eaffd50)         = 9520
set_robust_list(0x7f7d7eaffd60, 24)     = 0
rseq(0x7f7d7eb003a0, 0x20, 0, 0x53053053) = 0
mprotect(0x55f34ccf5000, 4096, PROT_READ) = 0
exit(42)                                = ?
+++ exited with 42 +++
```

## Memory (stack)

the stack fulfils four main uses:

- Track the "callstack" of a program.
    - return values are *pushed* to the stack during a call and "popped" during a ret.
- Contain local variables of functions.
- Provide scratch space (to alleviate register exhaustion).
- Pass fucntion arguments (always on x86, only for functions with "many" arguments on other architectures).

### Memory (Other mapped regions)

other regions might be mapped in memory.function such as `mmap` and `malloc` can cause other regions to be mapped as well.

### Memory (endianess)

Data on most modern system is stored backwards, in little endian.

> Why ??

- Performance (historical)
- Ease of addressing for different sizes.
- (apocryphal) 8086 compatibility.

## Signedness: Tow's Compliment

How to differentiate between positive and negative numbers?

One idea: signed bit (8bit example):

## Calling Conventions

Callee and caller functions must agree on argument passing.

## Other Resources

- Reppel
