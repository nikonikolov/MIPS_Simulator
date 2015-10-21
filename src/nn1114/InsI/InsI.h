
/* Definition I-type (instruction) with opcode and pointer to function implementation of actual instruction*/

#ifndef INSI_H
#define INSI_H
#include "../Ins/Ins.h"

typedef mips_error (*FPI)(uint32_t, uint16_t, uint64_t&);

template<class FnPtrSpec>
class InsI : public Ins<FnPtrSpec> {

public:
	
	InsI(uint8_t opcode_in, char* name_in, FnPtrSpec FnImpl_in):
	Ins<FnPtrSpec>(opcode_in, name_in, FnImpl_in) {}
	

	void debugPrintIns(mips_cpu_h state, uint32_t InsWord){

		if(state->logLevel == LOGMIDDLE){
        	/*fprintf(state->logDst, "R-Type : dst=%u, src1=%u, src2=%u, shift=%u, function=%u.\n  instr=%08x\n",
            							dst, src1, src2, shift, function, instruction
        	);*/
    	}

    	else if (state->logLevel == LOGHIGH){
    		debugPrintWord(state, InsWord);
    	}
	}

};

typedef InsI<FPI> InsObjI;

#endif