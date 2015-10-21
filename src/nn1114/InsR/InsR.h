
/* Definition R-type (instruction) with opcode and pointer to function implementation of actual instruction*/


#ifndef InsR_H
#define InsR_H
#include "mips.h"

typedef mips_error (*FPR)(uint32_t, uint32_t, uint64_t&, uint8_t);

class InsR{
public:
	InsR(uint8_t opcode_in, FPR FnImpl_in);

	uint8_t get_opcode() const;
	FPR get_FnImpl() const;

protected:
	uint8_t opcode;
	FPR FnImpl;
};

#endif