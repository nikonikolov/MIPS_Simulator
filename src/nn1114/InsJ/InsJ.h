
/* Definition J-type (instruction) with opcode and pointer to function implementation of actual instruction*/

#ifndef INSJ_H
#define INSJ_H
#include "../Ins/Ins.h"

typedef mips_error (*FPJ)(mips_cpu_h, uint32_t);

template<class FnPtrSpec>
class InsJ : public Ins<FnPtrSpec> {

public:
	
	InsJ(uint8_t opcode_in, char* name_in, FnPtrSpec FnImpl_in):
	Ins<FnPtrSpec>(opcode_in, name_in, FnImpl_in) {}

	~InsJ(){}

	mips_error debugIns(mips_cpu_h state, uint32_t InsWord, uint32_t result) const {

		if(state->logLevel >= LOGLOW){
			uint8_t code = extr_fn(InsWord);
			fprintf(state->logDst, "WordCode=0x%x, FoundCode=0x%x, FoundIns=%s\n", code, Ins<FnPtrSpec>::opcode, Ins<FnPtrSpec>::name);
			
			if(state->logLevel >= LOGMIDDLE){
        		uint32_t jarg = extr_jarg(InsWord);
			
        		fprintf(state->logDst, "J-Type : arg=%u\n", jarg);
    		
    			if (state->logLevel >= LOGHIGH){

    				mips_error err = printPC(state);
    				if(err) return err;

    				debugPrintWord(state, InsWord);
    			}
    		}
		}
		return mips_Success;
	}

};

typedef InsJ<FPJ> InsObjJ;

#endif