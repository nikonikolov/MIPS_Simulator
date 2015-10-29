#include "mips.h"
#include "mips_cpu_def.h"
#include "mips_cpu_private.h"
#include "mips_cpu_decode.h"
#include "mips_cpu_print.h"


mips_cpu_h mips_cpu_create(mips_mem_h mem){
	mips_cpu_h cpuPtr = new mips_cpu_impl;
	cpuPtr->PC = 0;
	cpuPtr->nPC = cpuPtr->PC + BLOCKSIZE;
	cpuPtr->branch = 4;
	for(int i=0; i<REGS; i++) cpuPtr->regs[i] = 0;
	cpuPtr->memPtr = mem;
	
	mips_cpu_set_debug_level(cpuPtr, 0, NULL);

	return cpuPtr;
}


mips_error mips_cpu_reset(mips_cpu_h state){
	
	state->PC = 0;
	state->nPC = state->PC + BLOCKSIZE;
	state->branch = 4;

	for(int i=0; i<REGS; i++) state->regs[i] = 0;

	mips_cpu_set_debug_level(state, 0, NULL);
			
	return mips_Success;
}


mips_error mips_cpu_set_debug_level(mips_cpu_h state, unsigned level, FILE *dest)
{
    state->logLevel = level;
    state->logDst = dest;
    return mips_Success;
}


mips_error mips_cpu_get_register(
	mips_cpu_h state,	//!< Valid (non-empty) handle to a CPU
	unsigned index,		//!< Index from 0 to 31
	uint32_t *value		//!< Where to write the value to
){
	if(index<REGS){
		*value = state->regs[index];
		return mips_Success;
	}

	else return printErr(state, mips_ErrorInvalidArgument, "Fn: mips_cpu_get_reg, reg idx not in range");	
}


mips_error mips_cpu_set_register(
	mips_cpu_h state,	//!< Valid (non-empty) handle to a CPU
	unsigned index,		//!< Index from 0 to 31
	uint32_t value		//!< New value to write into register file
){
	if(index<REGS){
		if(index!=0)	state->regs[index] = value;
		return mips_Success;
	}
	
	else return printErr(state, mips_ErrorInvalidArgument, "Fn: mips_cpu_set_reg, reg idx not in range");	
}


mips_error mips_cpu_set_pc(
	mips_cpu_h state,	//!< Valid (non-empty) handle to a CPU
	uint32_t pc			//!< Address of the next instruction to exectute.
){	
	// If address is not divisible by 4, error
	if(pc & 0x00000003) return mips_ExceptionInvalidAddress;
	
	state->PC = pc;
	// NOTE: THE EXPECTED BEHAVIOUR WOULD BE TO SET BOTH PC AND nPC
	state->nPC = state->PC + BLOCKSIZE;

	return mips_Success;
}


mips_error mips_cpu_get_pc(
	mips_cpu_h state,	//!< Valid (non-empty) handle to a CPU
	uint32_t *pc		//!< Where to write the byte address too
){
	*pc=state->PC;
	return mips_Success;
}


mips_error mips_cpu_step(
	mips_cpu_h state	//! Valid (non-empty) handle to a CPU
){
	
	uint32_t InsWord;

	mips_error err = mips_mem_read(
   	state->memPtr,							//!< Handle to target memory
    state->PC,								//!< Byte address to start transaction at
    (uint32_t)BLOCKSIZE,					//!< Number of bytes to transfer
    (uint8_t*)&(InsWord)					//!< Receives the target bytes
	);	
	if(err) return printErr(state, err, "Fn: mips_cpu_step, mem_read unsuccessful");	
	
	// convert from big endian
	InsWord = nn1114_change_endian(InsWord);


	// Find instruction type
	FP FnImpl = decodeType(InsWord);


	// decode and execute instruction
	err = FnImpl(state, InsWord);	
	
	// NOTE: YOU SHOULD NOT ADVANCE PC WHEN EXCEPTION OCCURS
	if(err) return printErr(state, err, "Fn: mips_cpu_step, find and execute unsuccessful");	


	// BRANCHES CHANGE nPC DIRECTLY SO THAT WHEN IT IS INCREMENTED HERE, IT RESULTS IN THE PROPER BRANCH ADDRESS
	advance_pc(state, state->branch);

	return mips_Success;	
}


void mips_cpu_free(mips_cpu_h state){
	delete state;
}

