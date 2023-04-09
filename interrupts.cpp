#include "interrupts.h"

void printf(char* str);


InterruptManager::GateDescriptor InterruptManager::interruptDescriptorTable[256];

InterruptManager* InterruptManager::ActiveInterruptManager = 0;

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
	interruptDescriptorTable[interruptNumber].access = IDT_DESC_PRESENT | ((DescriptorPrivilegeLevel & 3) << 5) | DescriptorType;


}
// 2) set all interrupts handler to IgnoreInterruptRequest()
//    then declare two handlers for timer and keyboard
// 	  then tell processot to load the IDT
// 4) Instantiate ports also in constructor
InterruptManager::InterruptManager(GlobalDescriptorTable* gdt)
: picMasterCommand(0x20), 
  picMasterData(0x21), 
  picSlaveCommand(0xA0), 
  picSlaveData(0xA1)
{
	uint16_t CodeSegment = gdt->CodeSegmentSelector();
	const uint8_t IDT_INTERRUPT_GATE = 0xE;
	for(uint8_t i = 255; i > 0; --i)
		SetInterruptDescriptorTableEntry(i, CodeSegment, &IgnoreInterruptRequest, 0, IDT_INTERRUPT_GATE);
		
	SetInterruptDescriptorTableEntry(0x20, CodeSegment, &HandleInterruptRequest0x00, 0, IDT_INTERRUPT_GATE);
	SetInterruptDescriptorTableEntry(0x21, CodeSegment, &HandleInterruptRequest0x01, 0, IDT_INTERRUPT_GATE);

	picMasterCommand.Write(0x11);
	picSlaveCommand.Write(0x11);
	
	// Interrupt 1 is also for exceptions, so we tell master pic to add 0x20, if exception 1 is received
	picMasterData.Write(0x20);    
	picSlaveData.Write(0x28);
	
	picMasterData.Write(0x04);    
	picSlaveData.Write(0x02);
	
	picMasterData.Write(0x01);    
	picSlaveData.Write(0x01);
	
	picMasterData.Write(0x00);
	picSlaveData.Write(0x00);


	InterruptDescriptorTablePointer idt;
	idt.size = 256*sizeof(GateDescriptor) - 1;
	idt.base = (uint32_t)interruptDescriptorTable;
	asm volatile("lidt %0" : : "m" (idt));
}

InterruptManager::~InterruptManager()
{

}

// 3) tell pic to start sending interrupts
void InterruptManager::Activate()
{
	if(ActiveInterruptManager != 0)
		ActiveInterruptManager->Deactivate();
	ActiveInterruptManager = this;
	asm("sti");
}
void InterruptManager::Deactivate()
{
	if(ActiveInterruptManager == this)
		ActiveInterruptManager = 0;
	asm("cli");
}



// will be called from the assembly program
// will invoke the non-static DoHandleInterrupt method
uint32_t InterruptManager::handleInterrupt(uint8_t interruptNumber, uint32_t esp)
{
	printf(" INTERRUPT MAIN");
	if(ActiveInterruptManager != 0)
	{
		return ActiveInterruptManager->DoHandleInterrupt(interruptNumber, esp);
	}
	return esp;
}

uint32_t InterruptManager::DoHandleInterrupt(uint8_t interruptNumber, uint32_t esp)
{
	if(interruptNumber != 0x20)
		printf("  TIMER");
		
	if(0x20 <= interruptNumber && interruptNumber < 0x30)
	{
		picMasterCommand.Write(0x20);
		if(0x28 <= interruptNumber)
			picSlaveCommand.Write(0x20);
	}
	printf(" DO INTERRUPT");
	return esp;
}
