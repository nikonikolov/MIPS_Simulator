#include "mips.h"
#include "mips_cpu_def.h"
#include "mips_cpu_private.h"
#include "mips_cpu_ins_decode.h"


mips_cpu_h mips_cpu_create(mips_mem_h mem){
	mips_cpu_h cpuPtr = new mips_cpu_impl;
	cpuPtr->PC = 0;
	cpuPtr->nPC = cpuPtr->PC + BLOCKSIZE;
	for(int i=0; i<REGS; i++) cpuPtr->regs[i] = 0;
	cpuPtr->memPtr = mem;
	
	mips_cpu_set_debug_level(cpuPtr, 0, NULL);

	return cpuPtr;
}


// YOU NEED TO ACCOUNT FOR UNSUCCESSFUL CASE HERE
mips_error mips_cpu_reset(mips_cpu_h state){
	
	if(state==NULL)	return mips_ErrorInvalidArgument;

	state->PC = 0;
	state->nPC = state->PC + BLOCKSIZE;

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
	if(value==NULL)	return mips_ErrorInvalidArgument;

	else{
		if(index<REGS){
			*value = state->regs[index];
			return mips_Success;
		}

		else return mips_ErrorInvalidArgument;
	} 
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
	
	else return mips_ErrorInvalidArgument;
}


// ACCOUNT FOR CASE WHEN ADDRESS IS OUT OF ALLOCATED MEMORY !!! - read instruction might be taking care of that
mips_error mips_cpu_set_pc(
	mips_cpu_h state,	//!< Valid (non-empty) handle to a CPU
	uint32_t pc			//!< Address of the next instruction to exectute.
){
	state->PC = pc;
	state->nPC = state->PC + BLOCKSIZE;

	return mips_Success;
}


mips_error mips_cpu_get_pc(
	mips_cpu_h state,	//!< Valid (non-empty) handle to a CPU
	uint32_t *pc		//!< Where to write the byte address too
){
	if(pc==NULL) return mips_ErrorInvalidArgument;
	else{
		*pc=state->PC;
		return mips_Success;
	}
}


// DECODE INSTRUCTION AND IMPLEMENT REQUIRED
mips_error mips_cpu_step(
	mips_cpu_h state	//! Valid (non-empty) handle to a CPU
){
	
	uint32_t InsWord;

	mips_error err = mips_mem_read(
   	state->memPtr,							//!< Handle to target memory
    state->PC,								//!< Byte address to start transaction at
    (uint32_t)BLOCKSIZE,					//!< Number of bytes to transfer
    (uint8_t*)&(InsWord)		//!< Receives the target bytes
	// CHECK THAT THE LAST PARAMETER WORKS PROPERLY
	);
	
	// check if read is sucessful
	if(!err) return err;
	
	// convert from big endian
	InsWord = change_endian(InsWord);

	// Find instruction type
	FP FnImpl = decodeType(InsWord);

	// decode and execute instruction
	err = FnImpl(state, InsWord);
	if(!err) return err;		// SHOULD YOU ADVANCE PC WHEN EXCEPTION OCCURS?

	// DON'T FORGET TO ACCOUNT FOR BRANCHES

	advance_pc(state, (uint32_t)BLOCKSIZE);
	return mips_Success;	
}


void mips_cpu_free(mips_cpu_h state){
	if(state!=NULL)	delete state;
}

