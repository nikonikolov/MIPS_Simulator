
/* Definition I-type (instruction) with opcode and pointer to function implementation of actual instruction*/

#ifndef INSI_H
#define INSI_H
#include "mips.h"

typedef mips_error (*FPI)(uint32_t, uint16_t, uint64_t&);

class InsI{
public:
	InsI(uint8_t opcode_in, FPI FnImpl_in);

	uint8_t get_opcode() const;
	FPI get_FnImpl() const;

protected:
	uint8_t opcode;
	FPI FnImpl;
};

#endif