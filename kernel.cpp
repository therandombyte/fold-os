#include "types.h"

/*  get the pointer to a memory location, 
	leave the high byte as is, combine that with string to be printed
*/
void printf(char* str) {
	static uint16_t* VideoMemory = (uint16_t*)0xb8000;
	for(int i = 0; str[i] != '\0'; ++i)
		VideoMemory[i] = (VideoMemory[i] & 0xFF00) | str[i];
}

typedef void (*constructor)();         // declaring a function pointer
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors()
{
	for(constructor* i = &start_ctors; i != &end_ctors; i++)
		(*i)();
}

/* bootloader has stored the multiboot structure in ram, 
   which we should accept as params to our main function
*/
extern "C" void kernelMain(void* multiboot_structure, uint32_t magicNumber) {
	printf("This is Fold-OS!!");

	while(1);
}


/*	VIDEO1:
1:  First, need assembly to set the "stack pointer" and then 
    invoke the kernel mail function, as C++ expects it
2:  The grub will not recognize our bin file as a kernel 
    unless we add the "multiboot sector" and the magic number
3:  The multiboot structure will be stored somewhere in RAM for 
    later use and address stored in ax register
    and the magic value in bx register.
    We will use this later to get the total RAM size
4.  Loader fill will compile but the kernel will not, 
	as printf function needs to be defined
5.  Tell the compiler there is no glibc, no exception handler
	no mem management
6.	Time for linker. KernelMain needs to have extern C

	VIDEO2:
1.	add start_ctors in kernel file, to invoke the global objects
	constructors and put between start_ctors and end_ctors

	VIDEO3:
1.	creating iso target in makefile
	means adding grub-rescue and virtualbox
	got error: grub-mkrescue: error: mformat invocation failed
	solution: apt install mtools 
*/


