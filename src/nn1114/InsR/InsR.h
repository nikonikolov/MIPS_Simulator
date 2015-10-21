
/* Definition R-type (instruction) with opcode and pointer to function implementation of actual instruction*/

#ifndef INSR_H
#define INSR_H
#include "../Ins/Ins.h"


typedef mips_error (*FPR)(uint32_t, uint32_t, uint64_t&, uint8_t);

class InsR : public Ins {
public:	
	InsR(uint8_t opcode_in, char* name_in, FPR FnImpl_in);

	FPR get_FnImpl() const;

	void debugPrintIns(mips_cpu_h state, uint32_t InsWord);

private:
	FPR FnImpl;
};

#endif