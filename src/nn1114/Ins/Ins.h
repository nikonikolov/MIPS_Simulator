
/* Definition of class Ins (instruction) with opcode and pointer to function implementation of actual instruction*/

#ifndef INS_H
#define INS_H
#include "mips.h"

typedef void (*FP)(mips_cpu_h, uint32_t);

class Ins{
public:
	Ins(uint8_t opcode_in, FP FnImpl_in);

	FP get_FnImpl() const;
	uint8_t get_opcode() const;

private:
	uint8_t opcode;
	FP FnImpl;
};

#endif