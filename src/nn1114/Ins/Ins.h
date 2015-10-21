
/* Definition of class Ins (instruction) with opcode and pointer to function implementation of actual instruction*/

#ifndef INS_H
#define INS_H
#include "mips.h"

class Ins{

public:

	Ins(uint8_t opcode_in, char* name_in);
	uint8_t get_opcode() const;
	char* get_name();
	virtual void debugPrintIns(mips_cpu_h state, uint32_t InsWord) =0;

protected:
	uint8_t opcode;
	char* name; 
};

#endif