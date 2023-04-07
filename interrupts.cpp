#include "interrupts.h"

void printf(char* str);


InterruptManager::GateDescriptor InterruptManager::interruptDescriptorTable[256];

// 1) assign inputs to struct
void InterruptManager::SetInterruptDescriptorTableEntry(
	uint8_t interruptNumber, uint16_t gdt_codeSegmentSelectorOffset,
	void (*handler)(), uint8_t DescriptorPrivilegeLevel,
	uint8_t DescriptorType)
{
	const uint8_t IDT_DESC_PRESENT = 0x80;  // WHAT IS THIS!!

	interruptDescriptorTable[interruptNumber].handlerAddressLowBits = ((uint32_t)handler) & 0xFFFF;
	interruptDescriptorTable[interruptNumber].handlerAddressHighBits = (((uint32_t)handler) >> 16) & 0xFFFF;
	interruptDescriptorTable[interruptNumber].gdt_codeSegmentSelector = gdt_codeSegmentSelectorOffset;
	interruptDescriptorTable[interruptNumber].reserved = 0;
	interruptDescriptorTable[interruptNumber].access = IDT_DESC_PRESENT | DescriptorType | ((DescriptorPrivilegeLevel & 3) << 5);


}
// 2) set all interrupts handler to IgnoreInterruptRequest()
//    then declare two handlers for timer and keyboard
// 	  then tell processot to load the IDT
// 4) Instantiate ports also in constructor
InterruptManager::InterruptManager(GlobalDescriptorTable* gdt)
: picMasterCommand(0x20), picMasterData(0x21), picSlaveCommand(0xA0), picSlaveData(0xA1)
{
	uint16_t CodeSegment = gdt->CodeSegmentSelector();
	const uint8_t IDT_INTERRUPT_GATE = 0xE;
	for(uint16_t i = 0; i < 256; i++)
		SetInterruptDescriptorTableEntry(i, CodeSegment, &IgnoreInterruptRequest, 0, IDT_INTERRUPT_GATE);
		
	SetInterruptDescriptorTableEntry(0x20, CodeSegment, &HandleInterruptRequest0x00, 0, IDT_INTERRUPT_GATE);
	SetInterruptDescriptorTableEntry(0x21, CodeSegment, &HandleInterruptRequest0x01, 0, IDT_INTERRUPT_GATE);

	picMasterCommand.Write(0x11);
	picSlaveCommand.Write(0x11);
	
	picMasterData.Write(0x20);    // Interrupt 1 is also for exceptions, so we tell master pic to add 0x20, if exception 1 is received
	picSlaveData.Write(0x28);
	
	picMasterData.Write(0x04);    // Interrupt 1 is also for exceptions, so we tell master pic to add 0x20, if exception 1 is received
	picSlaveData.Write(0x02);
	
	picMasterData.Write(0x01);    // Interrupt 1 is also for exceptions, so we tell master pic to add 0x20, if exception 1 is received
	picSlaveData.Write(0x01);
	
	picMasterData.Write(0x00);    // Interrupt 1 is also for exceptions, so we tell master pic to add 0x20, if exception 1 is received
	picSlaveData.Write(0x00);


	InterruptDescriptorTablePointer idt;
	idt.size = 256 * sizeof(GateDescriptor) - 1;
	idt.base = (uint32_t)interruptDescriptorTable;
	asm volatile("lidt %0" : : "m" (idt));
}

InterruptManager::~InterruptManager()
{

}

// 3) tell pic to start sending interrupts
void InterruptManager::Activate()
{
	asm("sti");
}

// will be called from the assembly program
uint32_t InterruptManager::handleInterrupt(uint8_t interruptNumber, uint32_t esp)
{
	printf("Handle Interrupt");
	return esp;
}
