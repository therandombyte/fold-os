#include "types.h"

#ifndef __GDT_H
#define __GDT_H

class GlobalDescriptorTable
{
public:
	class SegmentDescriptor
	{
	private:
		uint16_t limit_lo;  //limit
		uint16_t base_lo;   //ptr
		uint8_t base_hi;    // ptr
		uint8_t type;      //access rights
		uint8_t flags_limit_hi; //limit
		uint8_t base_vhi;      // ptr
	public:
		SegmentDescriptor(uint32_t base, uint32_t limit, uint8_t type);
		uint32_t Base();
		uint32_t Limit();
	}__attribute__((packed));

	SegmentDescriptor nullSegmentSelector;
	SegmentDescriptor unusedSegmentSelector;
	SegmentDescriptor codeSegmentSelector;
	SegmentDescriptor dataSegmentSelector;

public:
	GlobalDescriptorTable();
	~GlobalDescriptorTable();

	uint16_t CodeSegmentSelector();
	uint16_t DataSegmentSelector();

};

#endif
