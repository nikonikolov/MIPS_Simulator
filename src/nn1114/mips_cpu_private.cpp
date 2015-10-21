#include "mips_cpu_private.h"

uint32_t change_endian(uint32_t word){
	 return (word << 24) |
          ((word <<  8) & 0x00ff0000) |
          ((word >>  8) & 0x0000ff00) |
          ((word >> 24) & 0x000000ff);
}


// offset can be a value in register, hence 32bit
mips_error advance_pc (mips_cpu_h state, uint32_t offset){
	state->PC = state->nPC;
	state->nPC += offset;

	return mips_Success;
}

mips_error check_overflow(const uint64_t& result){
	if(result>0xFFFFFFFF) return mips_ExceptionArithmeticOverflow;
	else return mips_Success;
}


mips_error shift_check(const uint8_t& shift){
    if(shift!=0x00) return mips_ExceptionInvalidInstruction;
    return mips_Success;
}


mips_error mips_cpu_get_npc(
	mips_cpu_h state,	//!< Valid (non-empty) handle to a CPU
	uint32_t *npc		//!< Where to write the byte address too
){
	if(npc==NULL) return mips_ErrorInvalidArgument;
	else{
		*npc=state->nPC;
		return mips_Success;
	}
}


/* ************************* DEBUGGING *************************** */


mips_error printPC(mips_cpu_h state){
	uint32_t PC, nPC;
	mips_error err = mips_cpu_get_pc (state, &PC);
	if(!err) return err;
	err = mips_cpu_get_npc (state, &nPC);
	if(!err) return err;
	debugPrintWord(state, PC);
	debugPrintWord(state, nPC);
	return mips_Success;
}


void debugPrintWord(mips_cpu_h state, const uint32_t& InsWord){
	fprintf(state->logDst, "HEX 0x%x/n", InsWord);
	printBin(state, InsWord);
	fprintf(state->logDst, "/n");
}


void printBin(mips_cpu_h state, uint32_t InsWord){
	fprintf(state->logDst, "BINARY ");
	for(int i=0; i<32; i++){
    	if (InsWord<<i & 0x80000000) fprintf(state->logDst, "1");
    	else fprintf(state->logDst, "0");
    	if(i%4==3) fprintf(state->logDst, " ");
	}
	fprintf(state->logDst, "\n");
}