
/* Definition J-type (instruction) with opcode and pointer to function implementation of actual instruction*/

#ifndef INSJ_H
#define INSJ_H
#include "../Ins/Ins.h"


typedef mips_error (*FPJ)(uint32_t);

class InsJ : public Ins {
public:
	InsJ(uint8_t opcode_in, char* name_in, FPJ FnImpl_in);

	FPJ get_FnImpl() const;
	
	void debugPrintIns(mips_cpu_h state, uint32_t InsWord);

private:
	FPJ FnImpl;
};

#endif