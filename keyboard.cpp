#include "keyboard.h"

void printf(char* str);

KeyboardDriver::KeyboardDriver(InterruptManager* manager)
:InterruptHandler(0x21, manager),
dataport(0x60),
commandport(0x64)
{
	while(commandport.Read() & 0x1)
		dataport.Read();

	commandport.Write(0xAE);  // start sending the interrupt
	commandport.Write(0x20);  // get current state
	uint8_t status = (dataport.Read() | 1) & ~0x10;  // change the state
	commandport.Write(0x60); // and write the new state back
	dataport.Write(status);

	dataport.Write(0xF4); // activate the keyboard
}

KeyboardDriver::~KeyboardDriver()
{

}

uint32_t KeyboardDriver::HandleInterrupt(uint32_t esp)
{
	uint8_t key = dataport.Read();   // Fetch the keystrike if any

	if(key < 0x80)    // ignore key release interrupts
	{
		switch(key)
		{
			case 0xFA: break;
			case 0x1E: printf("a"); break;
			case 0x45: case 0xC5: break;
			default:
				char* foo = "KEYBOARD 0x00 ";
				char* hex = "0123456789ABCDEF";
				foo[11] = hex[(key >> 4) & 0x0F];
				foo[12] = hex[key & 0x0F];
				printf(foo);
				break;
		}
	}
	
	return esp;
}
