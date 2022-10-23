# Fundamental 

## Computer Architecture

sources  -> Intermediate Languages bytecode -> Binary-encoded Instructions 
(pythons/Javascript/Java -> Interpreter/ JIT -> CPU)
(C/C++/Rust -> compiler -> CPU)



## Binary file

> What is ELF file?

ELF is a binary file format

Contains the program and its data.
Describes how the program should be loaded (program/segment headers).
Contains metadata describing program components (section headers).


```console
readelf -a cat

```

> ELF File Program Header

Program header specify information needed to prepare the program for execution. Most important entry types:

**INTERP**: defines the library that should be used to load this `ELF` into memory.
**LOAD**: defines a part of the file that should be loaded into memory.

Program headers are the source of information used when loading a file.


[kaitai.io/](https://ide.kaitai.io/)


> ELF Section Headers

A different view of the ELF with useful information for introspection, debugging etc.
Important sections:

`.text`:  the executable code of your program.
`.plt` and `.got`: used to resolve and dispatch library calls.
`.data`: used for pre-initialized global writable data (such as global arrays with initial values)
`.rodata`: used for global read-only data (such as strings constants)
`.bss`: used for uninitialized global writable data (such as global arrays without initial values)


> Symbol
Binary (and library) that use dynamically loaded libraries rely symbols (names) to find libraries, resolve function calls into those libraries,etc.

> Interacting with ELF

- **gcc** to make ELF (compiler)
- **readelf** ro parse the ELF header.
- **objdump** to parse the ELF header and disassemble the source code.
- **nm** to view ELF symbols.
  - `mn -a [bin]`
  - `mn -D [bin]`
- **patchelf** to change some ELF properties.
- **objcopy** to swap out ELF sections.
  - `objcopy --dump-section .rodata=data.dump [bin]`
- **strip** to remove otherwise-helpful information.


```bash

objcopy --dump-section .rodata=cat.dump cat

vim -b cat.dump

objcopy --update-section .rodata=cat.dump cat


strip cat 

nm -a cat 

```

## Linux Process Loading

### cat /flag

> Portrait of a process

Every Linux process has::

- State (running, waiting, stopped, zombie)
- Priority (and other scheduling information)
- parent, siblings, children
- shared resources (file, pipes, sockets)
- virtual memory space
- security context
  - effective uid and gid
  - saved uid and gid
  - capabilities

**fork** and **clone** are system calls that create a nearly exact copy of the calling process: a parent and a child.
Later, the child process usually used the `execve` `syscall` to replace itself with another process.

Example:
  - you type `/bin/cat` in bash
  - bash `forks` itself into the old parent process and the child process
  - The child process `execve` `/bin/cat`, becoming `/bin/cat`

Before anything is loaded, the kernel checks for executable permissions. if a file is not executable, `execve` will fail.
To figure out what to load, the linux kernel reads the beginning of the file and makes a decision.
1. If the file start with `#!` the kernel extracts the interpreter from the rest of that line and executes this interpreter with the original file as a argument.
2. If the file matches a format in `/proc/sys/fs/binfmt_misc`, the kernel executes the interpreter specified for that format with the original file as an argument.
3. If the file is a dynamically-linked `ELF`, the kernel reads the interpreter/loader defines in the `ELF`, loads the interpreter and the original file, and lets the interpreter take control.
4. If the file is a statically-linked `ELF`, the kernel will load it.
5. Other legacy file formats are checked for.


### Dynamically linked ELFs: the interpreter

process loading is done by the `ELF` interpreter specified in the binary.

```bash
$ readelf -a /bin/cat | grep interpret 

```

Can be overridden: `/lib64/ld-linux-x86-64..so.2 /bin/cat /flag`
Or changed permanently: `patchelf --set-interpreter`

### Dynamically linked ELFs: the loading process

1. The program and its interpreter are loaded by the kernel.
2. The interpreter located the libraries.
   1. `LD_PRELOAD` environment variable, and anything is `/etc/ld.so.preload`
   2. `LD_LIBRARY_PATH` environment variable (can be set in the shell)
   3. `LD_RUNPATH` or `DT_PATH` specified in the binary file (both can be modified with patchelf)
   4. system-wide configuration (/etc/ld.so.conf)
   5. `/lib` and `/usr/lib`
3. The interpreter loads the libraries
   1. these libraries can depend on other libraries, causing more to be loaded.
   2. relocations updates.



> preload.c

```c
int read(int fd, char *buf, int n){
  buf[0] = 'p';
  buf[1] = 'w';
  buf[2] = 'n';
  buf[3] = 'e';
  buf[4] = 'd';
  buf[5] = '\n';
  return 6;
}

```


> Compile it as libraries
```console
gcc -shared -o preload.so preload.c
```

> run preload on .cat
```bash
LD_PRELOAD=./preload.so ./cat
```

> See what happened by `strace`
```bash
strace -E LD_PRELOAD=./preload.so ./cat 2>&1 | head -100
```

> load rpath 
```bash
patchelf --set-rpath /some/path ./cat
```

### Where is all this getting loaded to?

Each linux process has a virtual memory space. it contains:

  - the binary
  - the libraries
  - the "heap" (for dynamically allocated memory)
  - the "stack" (for function local variable)
  - any memory specified mapped by the program
  - some helper regions
  - kernel code in the "upper half" of memory (above 0x8000000000000000 on 64-bit architectures) inaccessible to the process

Virtual memory is dedicated to your process.
Physical memory is shared among the whole system.

You can see this whole space by looking at `/proc/self/maps`

### What is `libc.so`

libc.so is linked by almost every process.
Provides functionality you take for granted:
- `printf()`
- `scanf()`
- `socket()`
- `atoi()`
- `malloc()`,
- `free()`
- `xdr_keystatus()`


```bash
gcc -static -o cat-static cat.c


du -sbh cat-static cat
```

## Initialization

Every ELF binary can specify contractors

```c
__attribute__((contractor)) void hahaha(){
  write(1, "haha!\n", 6);
}

```