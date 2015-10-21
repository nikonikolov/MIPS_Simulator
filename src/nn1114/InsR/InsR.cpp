#include "InsR.h"

InsR::InsR(uint8_t opcode_in, char* name_in, FPR FnImpl_in) :
	Ins(opcode_in, name_in), FnImpl(FnImpl_in) {}


FPR InsR::get_FnImpl() const{
	return FnImpl;
}


void InsR::debugPrintIns(mips_cpu_h state, uint32_t InsWord){

	if(state->logLevel == LOGMIDDLE){
        /*fprintf(state->logDst, "R-Type : dst=%u, src1=%u, src2=%u, shift=%u, function=%u.\n  instr=%08x\n",
            							dst, src1, src2, shift, function, instruction
        );*/
    }

    else if (state->logLevel == LOGHIGH){
    	debugPrintWord(state, InsWord);
    }
}