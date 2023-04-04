# fold-os

A Kernel/OS from Scratch in C++ for learning purposes.
Inspired by http://algorithman.de/

Technical Challenges in writin a Kernel:
1: First, we need assembly language to set the "stack pointer" and then
invoke the kernel mail function, as C++ expects it
2: The grub will not recognize our bin file as a kernel
unless we add the "multiboot sector" and the magic number
3: The multiboot structure will be stored somewhere in RAM for
later use and address stored in ax register
and the magic value in bx register.
We will use this later to get the total RAM size 4. Loader fill will compile but the kernel will not,
as printf function needs to be defined 5. Tell the compiler there is no glibc, no exception handler
no mem management 6. Time for linker. KernelMain needs to have extern C

7.  add start_ctors in kernel file, to invoke the global objects
    constructors and put between start_ctors and end_ctors

8.  creating iso target in makefile
    means adding grub-rescue and virtualbox
    got error: grub-mkrescue: error: mformat invocation failed
    solution: apt install mtools

9.  GDT:
    Single code and data segment implemented in the descriptor table
    there should be two, one for kernel and one for user mode.
    10.GDT should have function to return offset of DS and CS
    11.The segement selector (entries in GDT) should return
    the base and limit offset
    12.Weird Limit logic of 12 virtual bytes being 1

        VIDEO5:

10. Talking to PIC, using "outb"
11. Port implementation in OOPS way
12. Add "clean" target and enhance printf to print multiple lines

    VIDEO6:

13. IDT
