#ifndef __INTERRUPTMANAGER_H
#define __INTERRUPTMANAGER_H

#include "types.h"
#include "port.h"
#include "gdt.h"

// not the IDT but a manager of it?
class InterruptManager 
{
protected:
	// 4) a static pointer to be called from C++ static function
	// 	  thats invoked by assembler
	static InterruptManager* ActiveInterruptManager;

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
	void Deactivate();

	// 2) implementation of these handlers (forward declared) are in the assemble macro
	//    so handlers written in C++ will be used in assembler as macro
	static void IgnoreInterruptRequest();
	static void HandleInterruptRequest0x00();
	static void HandleInterruptRequest0x01();
	static void HandleException0x00();
    static void HandleException0x01();

	// 1) this will be called from the assembly program for each interrupt
	static uint32_t handleInterrupt(uint8_t interruptNumber, uint32_t esp);
	// 5) this will be invoked by static handleInterrupt method
	uint32_t DoHandleInterrupt(uint8_t interruptNumber, uint32_t esp);


};

#endif