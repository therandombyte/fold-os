#include "types.h"

#ifndef __GDT_H
#define __GDT_H

class GlobalDescriptor
{
	class SegmentDescriptor
	{
		uint16_t limit_lo;  //limit
		uint16_t base_lo;   //ptr
		uint8_t base_hi;    // ptr
		uint8_t type;      //access rights
		uint8_t flags_limit_hi; //limit
		uint8_t base_vhi;      // ptr

	}__attribute__((packed));

};

#endif
