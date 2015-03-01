Porting to Different Architectures
===================

In `kernel/` there is a folder called arch that contains subdirectories for each architecture that Cedille supports. In these folders is a mirror of the `kernel/` directory tree, as well as _boards_, sub-architectures that have there own code as well. Each directory gets more specialised, the code in `arch/arm/` will work for all ARM processors, but the code in `arch/arm/rpi/` will only work on the Raspberry Pi. 

In this example, the Raspberry Pi is compiled with all the code in `arch/arm/rpi`, the (general) code in `arch/arm/` as well as the main kernel code. Each subdirectory's structure is a mirror of `kernel/`, so that files that would be in `init` that relate to the Raspberry Pi would go in `arch/arm/rpi/init/`. It also has the subdirectory `drivers` which stores drivers for each board and arch. It is also required that a linker file be placed in the root of the board titled `link.ld`. This is board specific.

Code requirements
--------------------
> Note: This is all board specific, and is relative to the actual board directory.

1. The code must start in assembly (in `init/start.s`, yes `.s`). 
Prefer nasm, then yasm, then as. The entry point is `_start` and should setup the stack. Then it calls `kernel_entry()`
2. `kernel_entry()` must be void, and stored in `init/entry.c`. 
It needs to initialise the board to a point where all interrupts, paging, and other board setup is done and handled properly.
It can contain parameters, like r0,r1,r2 on ARM and multiboot data on x86
3. `kernel_entry()` needs to then hand control over to `kmain()`.
Do not expect `kmain()` to return. The execution is mostly out of your hands now.
4. Keep all architecture specific code out of `kernel/` unless it is necessary and wrapped in `#ifdef ARCH${NAME}` blocks.
An example of this is in the text console, we need double space for attribute bytes, so this is an acceptable case.

Building
--------------------
> Note: This applies to everyone in general, should be moved to a new document.

1. Get a working implementation of gcc / clang / whatever you use to compile.
2. Make sure it cross compiles correctly, and targets the correct thing.
3. Determine an output system (iso,image?). If you can't decide, just build an elf for now. (Use `make kernel`)
4. Build the kernel by passing the compiler options directly in from the command line.

Examples of Building
--------------------

On my host machine for building x86:
`make CC=i686-elf-gcc LD=i686-elf-gcc GENISO=genisoimage` which I have aliased to `make_kernel` in bash

You are required to pass `ARCH` and `BOARD`, which state what ARCH you are building, and what BOARD in ARCH were building (`ARCH/BOARD/`).
They are by default `x86` and `i386`, which is why I don't need to pass them in the example.

You can tune the following parameters:

* COMPILE_OPTIONS (for selective compiles)
* AS
* CC
* STRIP
* NM
* LD (set to CC if gcc)
* LFLAGS
* C_OPTIMIZ
* GENISO
* GENISOF
* EMU (emulator)
