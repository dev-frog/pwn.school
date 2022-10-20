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

