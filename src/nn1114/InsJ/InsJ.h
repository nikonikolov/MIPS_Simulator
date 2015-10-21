
/* Definition J-type (instruction) with opcode and pointer to function implementation of actual instruction*/

#ifndef INSJ_H
#define INSJ_H
#include "mips.h"

typedef mips_error (*FPJ)(uint32_t);

class InsJ{
public:
	InsJ(uint8_t opcode_in, FPJ FnImpl_in);

	uint8_t get_opcode() const;
	FPJ get_FnImpl() const;

protected:
	uint8_t opcode;
	FPJ FnImpl;
};

#endif