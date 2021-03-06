
/* Definition of class Ins (instruction) with opcode and pointer to function implementation of actual instruction*/

#ifndef INS_H
#define INS_H
#include "mips.h"
#include "../mips_cpu_def.h"
#include "../mips_cpu_private.h"
#include "../mips_cpu_print.h"


template <class FNPTR>
class Ins{

public:

	Ins(uint8_t opcode_in, char* name_in, FNPTR FnImpl_in) :
	opcode(opcode_in), name(name_in), FnImpl(FnImpl_in) {}

	virtual ~Ins(){}

	
	uint8_t get_opcode() const{
		return opcode;
	}
	
	char* get_name(){
		return name;
	}

	
	FNPTR get_FnImpl() const{
		return FnImpl;
	}

	virtual mips_error debugIns(mips_cpu_h state, uint32_t InsWord, uint32_t result) const =0;

protected:
	uint8_t opcode;
	char* name;
	FNPTR FnImpl; 
};

#endif
