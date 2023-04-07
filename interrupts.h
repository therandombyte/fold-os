#ifndef __INTERRUPTS_H
#define __INTERRUPTS_H

#include "types.h"
#include "port.h"
#include "gdt.h"

// not the IDT but a manager of it?
class InterruptManager 
{
protected:
	struct GateDescriptor
	{
		uint16_t handlerAddressLowBits;
		uint16_t gdt_codeSegmentSelector;
		uint8_t reserved;
		uint8_t access;
		uint16_t handlerAddressHighBits;
	}__attribute__((packed));

	static GateDescriptor interruptDescriptorTable[256];

	struct InterruptDescriptorTablePointer
	{
		uint16_t size;
		uint32_t base;
	}__attribute__((packed));

	static void SetInterruptDescriptorTableEntry(
		uint8_t interruptNumber, uint16_t gdt_codeSegmentSelectorOffset,
		void (*handler)(), uint8_t DescriptorPrivilegeLevel,
		uint8_t DescriptorType
	);

	Port8BitSlow picMasterCommand;
	Port8BitSlow picMasterData;
	Port8BitSlow picSlaveCommand;
	Port8BitSlow picSlaveData;

	

public:
	InterruptManager(GlobalDescriptorTable* gdt);
	~InterruptManager();

	// 3) tell pic to start sending interrupts
	void Activate();

	// 1) this will be called from the assembly program for each interrupt
	static uint32_t handleInterrupt(uint8_t interruptNumber, uint32_t esp);

	// 2) implementation of these handlers (forward declared) are in the assemble macro
	//    so handlers written in C++ will be used in assembler as macro
	static void IgnoreInterruptRequest();
	static void HandleException();
	static void HandleInterruptRequest0x00();
	static void HandleInterruptRequest0x01();

};



#endif
