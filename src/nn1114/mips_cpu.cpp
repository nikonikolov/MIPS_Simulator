#include "mips.h"
#include "mips_cpu_private.h"
#define BLOCKSIZE 4
#define REGS 32

struct mips_cpu_impl{

	uint32_t pc;
	uint32_t regs[REGS];
	mips_mem_h memPtr;
	uint32_t instruction;
};


mips_cpu_h mips_cpu_create(mips_mem_h mem){
	mips_cpu_h cpuPtr = new mips_cpu_impl;
	cpuPtr->pc = 0;
	for(int i=0; i<REGS; i++) cpuPtr->regs[i] = 0;
	cpuPtr->memPtr = mem;

	return cpuPtr;
}

// YOU NEED TO ACCOUNT FOR UNSUCCESSFUL CASE HERE
mips_error mips_cpu_reset(mips_cpu_h state){
	
	if(state==NULL)	return mips_ErrorInvalidArgument;

	else{
		state->pc = 0;
		for(int i=0; i<REGS; i++) state->regs[i] = 0;
		return mips_Success;
	}
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
	state->pc = pc;
	return mips_Success;
}


mips_error mips_cpu_get_pc(
	mips_cpu_h state,	//!< Valid (non-empty) handle to a CPU
	uint32_t *pc		//!< Where to write the byte address too
){
	if(pc==NULL) return mips_ErrorInvalidArgument;
	else{
		*pc=state->pc;
		return mips_Success;
	}
}


// DECODE INSTRUCTION AND IMPLEMENT REQUIRED
mips_error mips_cpu_step(
	mips_cpu_h state	//! Valid (non-empty) handle to a CPU
){
	mips_error err = mips_cpu_set_pc( state, (state->pc)+BLOCKSIZE );
	if(err!=mips_Success) return err;
	else{
		mips_mem_read(
   		state->memPtr,							//!< Handle to target memory
    	state->pc,								//!< Byte address to start transaction at
    	(uint32_t)BLOCKSIZE,					//!< Number of bytes to transfer
    	(uint8_t*)&(state->instruction)		//!< Receives the target bytes
			// CHECK THAT THE LAST PARAMETER WORKS PROPERLY
		);

		// HAVE TO CHECK WHETHER READ WAS SUCCESSFUL

		state->instruction = change_endian(state->instruction);
		return mips_Success;
	}


}


void mips_cpu_free(mips_cpu_h state){
	if(state!=NULL)	delete state;
}

