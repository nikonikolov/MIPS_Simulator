
/* Definition I-type (instruction) with opcode and pointer to function implementation of actual instruction*/

#ifndef INSR_H
#define INSR_H
#include "../Ins/Ins.h"
//#include "../mips_cpu_ins_decode.h"

typedef mips_error (*FPR)(uint32_t, uint32_t, uint64_t&, uint8_t);

template<class FnPtrSpec>
class InsR : public Ins<FnPtrSpec> {

public:
	
	InsR(uint8_t opcode_in, char* name_in, FnPtrSpec FnImpl_in):
	Ins<FnPtrSpec>(opcode_in, name_in, FnImpl_in) {}
	

	mips_error debugPrintIns(mips_cpu_h state, uint32_t InsWord){

		if(state->logLevel >= LOGLOW){
			uint8_t code = extr_fn(InsWord);
			fprintf(state->logDst, "WordCode=0x%x, FoundCode=0x%x, FoundIns=%s\n", code, Ins<FnPtrSpec>::opcode, Ins<FnPtrSpec>::name);
			
			if(state->logLevel >= LOGMIDDLE){
        		uint8_t rs = extr_src1(InsWord);
				uint8_t rt = extr_src2(InsWord);
				uint8_t rd = extr_dest(InsWord);
				uint8_t shift = extr_shift(InsWord);
			
        		fprintf(state->logDst, "R-Type : rs=%u, rt=%u, rd=%u, shift=%u\n", rs, rt, rd, shift);
    		
    			if (state->logLevel >= LOGHIGH){

    				uint32_t src1, src2;
    				mips_error err = mips_cpu_get_register( state, rs, &src1);
    				if(!err) return err;
    				err = mips_cpu_get_register( state, rt, &src2); 
    				if(!err) return err;

    				fprintf(state->logDst, " src1=%u, src2=%u\n", src1, src2);

    				debugPrintWord(state, InsWord);
    			}
    		}
		}
	}

};

typedef InsR<FPR> InsObjR;

#endif