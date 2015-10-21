
/* Definition I-type (instruction) with opcode and pointer to function implementation of actual instruction*/

#ifndef INSI_H
#define INSI_H
#include "../Ins/Ins.h"
#include "../mips_cpu_def.h"
#include "../mips_cpu_private.h"


typedef mips_error (*FPI)(uint32_t, uint16_t, uint64_t&);

class InsI : public Ins {
public:
	InsI(uint8_t opcode_in, char* name_in, FPI FnImpl_in);

	FPI get_FnImpl() const;
	
	void debugPrintIns(mips_cpu_h state, uint32_t InsWord);

private:
	FPI FnImpl;
};

#endif