#include "types.h"
#include "gdt.h"

/*  get the pointer to a memory location, 
	leave the high byte as is, combine that with string to be printed
	Enhancement: print at the cursor instead of always at the default location
	then move the cursor
*/
void printf(char* str) {

	static uint8_t x=0, y=0;
	static uint16_t* VideoMemory = (uint16_t*)0xb8000;
	
	for(int i = 0; str[i] != '\0'; ++i)
	{
		switch(str[i])
		{
			case '\n':
				y++;
				x = 0;
				break;
			default:
				VideoMemory[80*y + x] = (VideoMemory[80*y + x] & 0xFF00) | str[i];
				x++;
				break;
		}
		
		if(x >= 80)
		{
			y++;
			x = 0;
		}

		// reached end of screen, loop from the start and clear the screen
		if (y >= 25) {
			for(y = 0; y < 25; y++)
				for(x = 0; x < 80; x++)
					VideoMemory[80*y + x] = (VideoMemory[80*y + x] & 0xFF00) | ' ';
			x = 0;
			y = 0;
		}
	}		
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
	printf("This is Fold-OS!!\n");
	printf("Version 0.1");
	GlobalDescriptorTable gdt;
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

	VIDEO4:
1.	GDT: 
	Single code and data segment implemented in the descriptor table
	there should be two, one for kernel and one for user mode.
2.	GDT should have function to return offset of DS and CS
3. 	The segement selector (entries in GDT) should return
	the base and limit offset
4.	Weird Limit logic of 12 virtual bytes being 1

	VIDEO5:
1.	Talking to PIC, using "outb"
2.	Port implementation in OOPS way
3.	Add "clean" target and enhance printf to print multiple lines

	VIDEO6:
1.	IDT
*/


