#include "mips_cpu_private.h"

/* *********************** HELP FUNCTIONS NEEDED ONLY IN THE FUNCTIONS IN THE CORRESPONDING HEADER *********************** */ 

/*static void printBin(mips_cpu_h state, uint32_t InsWord, char* id){
	
	if(id!=NULL) fprintf(state->logDst, "%s: ", id);
	
	fprintf(state->logDst, "BINARY ");
	for(int i=0; i<32; i++){
    	if (InsWord<<i & 0x80000000) fprintf(state->logDst, "1");
    	else fprintf(state->logDst, "0");
    	if(i%4==3) fprintf(state->logDst, " ");
	}
	fprintf(state->logDst, "\n");
}*/


bool check_negative(const uint32_t& arg){
	if (arg<=0x7FFFFFFF) return 0;
	else return 1;
}

/* *********************** END HELP FUNCTIONS NEEDED ONLY IN THE FUNCTIONS IN THE CORRESPONDING HEADER *********************** */ 


uint32_t nn1114_change_endian(uint32_t word){
	 return ((word << 24) & 0xff000000) |
          	((word <<  8) & 0x00ff0000) |
          	((word >>  8) & 0x0000ff00) |
          	((word >> 24) & 0x000000ff);
}

uint32_t sign_extend(uint16_t arg){
	if(arg & 0x8000) return 	(0xFFFF0000) |
								((arg<<0) 	& 0x0000FFFF);
	else return (uint32_t)arg;
}

// offset can be a value in register, hence 32bit
mips_error advance_pc (mips_cpu_h state, uint32_t offset){
	state->PC = state->nPC;
	state->nPC += offset;
	state->branch = 4;

	return mips_Success;
}


mips_error check_overflow(const uint32_t& src1, const uint32_t& src2, const uint32_t& result){

	bool pos_overflow = (!check_negative(src1)) && (!check_negative(src2)) && (check_negative(result)) ;
	bool neg_overflow = (check_negative(src1)) && (check_negative(src2)) && (!check_negative(result)) ;

	if(pos_overflow || neg_overflow) return mips_ExceptionArithmeticOverflow;
	
	return mips_Success;
}


mips_error argzerocheck(const uint8_t& arg){
    if(arg!=0x00) return mips_ExceptionInvalidInstruction;
    return mips_Success;
}


mips_error nn1114_mips_cpu_get_npc(
	mips_cpu_h state,	//!< Valid (non-empty) handle to a CPU
	uint32_t *npc		//!< Where to write the byte address too
){
	if(npc==NULL) return mips_ErrorInvalidArgument;
	else{
		*npc=state->nPC;
		return mips_Success;
	}
}

mips_error mips_cpu_set_npc(
	mips_cpu_h state,		//!< Valid (non-empty) handle to a CPU
	uint32_t npc			//!< Address of the next instruction to exectute.
){	
	// If address is not divisible by 4, error
	if(npc & 0x00000003) return mips_ExceptionInvalidAddress;
	
	state->nPC = npc;
	return mips_Success;
}


mips_error mips_cpu_set_branch(
	mips_cpu_h state,		//!< Valid (non-empty) handle to a CPU
	uint32_t branch			//!< Address of the next instruction to exectute.
){	
	// If address is not divisible by 4, error
	if(branch & 0x00000003) return mips_ExceptionInvalidAddress;
	
	state->branch = branch;
	return mips_Success;
}

/* ************************* WORD EXTRACTION *************************** */
uint8_t extr_opcode(uint32_t InsWord){
	return ( (InsWord>>26) & 0x0000003F );
}

uint8_t extr_src1(uint32_t InsWord){
    return ( (InsWord>> 21 ) & 0x0000001F );	 
}

uint8_t extr_src2(uint32_t InsWord){
    return ( (InsWord>> 16 ) & 0x0000001F );	 
}

uint8_t extr_dest(uint32_t InsWord){
    return ( (InsWord>> 11 ) & 0x0000001F );	 
}

uint8_t extr_shift(uint32_t InsWord){
    return ( (InsWord>> 6 ) & 0x0000001F );	 
}

// Extract function type of R-type instruction 
uint8_t extr_fn(uint32_t InsWord){
	return (InsWord & 0x0000003F );
}

uint16_t extr_imm(uint32_t InsWord){
	return (InsWord & 0x0000FFFF );
}

uint32_t extr_jarg(uint32_t InsWord){
	return (InsWord & 0x03FFFFFF );
}

/* ************************* DEBUGGING *************************** */

/*
mips_error printPC(mips_cpu_h state){
	uint32_t PC, nPC;
	
	mips_cpu_get_pc (state, &PC);
	nn1114_mips_cpu_get_npc (state, &nPC);
	
	debugPrintWord(state, PC, "PC");
	debugPrintWord(state, nPC, "nPC");
	return mips_Success;
}

*/
//void debugPrintWord(mips_cpu_h state, const uint32_t& InsWord, char* id /*=NULL*/){
/*	if(id!=NULL) fprintf(state->logDst, "%s: ", id);
	fprintf(state->logDst, "HEX 0x%x\n", InsWord);
	printBin(state, InsWord, id);
	fprintf(state->logDst, "\n");
}

mips_error printErr(mips_cpu_h state, mips_error err, char* msg){
	if(state->logLevel>=LOGERROR){
		fprintf(state->logDst, "%s\n", msg);
		fprintf(state->logDst, "Error code %x\n", err);
	}
	return err;	
}
*/
