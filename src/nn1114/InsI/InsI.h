
/* Definition I-type (instruction) with opcode and pointer to function implementation of actual instruction*/

#ifndef INSI_H
#define INSI_H
#include "../Ins/Ins.h"

typedef mips_error (*FPI)(uint32_t, uint32_t, uint32_t&);

template<class FnPtrSpec>
class InsI : public Ins<FnPtrSpec> {

public:
	
	InsI(uint8_t opcode_in, char* name_in, FnPtrSpec FnImpl_in):
	Ins<FnPtrSpec>(opcode_in, name_in, FnImpl_in) {}

	~InsI(){}	
	
	mips_error debugIns(mips_cpu_h state, uint32_t InsWord, uint32_t result) const {

		if(state->logLevel >= LOGLOW){
			uint8_t code = extr_opcode(InsWord);
			fprintf(state->logDst, "WordCode=0x%x, FoundCode=0x%x, FoundIns=%s\n", code, Ins<FnPtrSpec>::opcode, Ins<FnPtrSpec>::name);
			
			if(state->logLevel >= LOGMIDDLE){
        		uint8_t rs = extr_src1(InsWord);
				uint8_t rd = extr_src2(InsWord);
				uint16_t imm = extr_imm(InsWord);
			
        		fprintf(state->logDst, "I-Type : rs=%u, imm=%u, rd=%u\n", rs, imm, rd);
        		fprintf(state->logDst, "Calculated Result=%u\n", result);
    		
    			if (state->logLevel >= LOGHIGH){

    				uint32_t src1;
    				mips_error err = mips_cpu_get_register( state, rs, &src1);
    				if(err) return err;

    				fprintf(state->logDst, " src1=%u\n", src1);

    				debugPrintWord(state, InsWord);
    			}
    		}
		}
		return mips_Success;
	}

};

typedef InsI<FPI> InsObjI;

#endif