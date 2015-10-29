#include "mips_cpu_ins_set.h"

// NOTE: 0x80000000 returns 0x80000000
static uint32_t twoscomp(uint32_t number){
	return ((0xFFFFFFFF^number) + 1);
}

static uint32_t zero_extend(uint32_t arg){
	return 	(0x0000FFFF & arg);
}

static bool compSigned(uint32_t src1, uint32_t src2){
	bool s1_neg = 0x80000000 & src1;
	bool s2_neg = 0x80000000 & src2;
	
	// src1 and src2 both negative
	if(s1_neg && s2_neg){
		// compare unsigned
		if(src2>src1) return true;
		else return false;
	}

	// src1 and/or src2 is positive
	if(s1_neg) return true;

	// src1 is 0 or positive, src2 negative
	if(s2_neg) return false;

	// both positive
	if(src1<src2) return true;

	// src1>=src2
	return false;
}

DEFR(add){
	
	mips_error err = argzerocheck(shift);
	if(err) return err;
	
	// do arithmetics
	result = src1 + src2;
	
	// check overflow
	err = check_overflow(src1, src2, result);
	if(err) return err;

	return mips_Success;
}


DEFR(addu){

    mips_error err = argzerocheck(shift);
	if(err) return err;

	// do arithmetics
	result = src1 + src2;
	
	return mips_Success;
}


DEFR(sub){
	return add(state, src1, twoscomp(src2), result, shift, rs, rt, rd);
}

DEFR(subu){
	return addu(state, src1, twoscomp(src2), result, shift, rs, rt, rd);
}

DEFR(andr){
    mips_error err = argzerocheck(shift);
	if(err) return err;

	// do arithmetics
	result = src1 & src2;

	return mips_Success; 
}

DEFR(orr){
    mips_error err = argzerocheck(shift);
	if(err) return err;

	// do arithmetics
	result = src1 | src2;

	return mips_Success; 
}

DEFR(xorr){
    mips_error err = argzerocheck(shift);
	if(err) return err;

	// do arithmetics
	result = src1 ^ src2;

	return mips_Success; 
}

DEFR(divr){ return mips_ErrorNotImplemented; }
DEFR(divu){ return mips_ErrorNotImplemented; }
DEFR(mult){ return mips_ErrorNotImplemented; }
DEFR(multu){ return mips_ErrorNotImplemented; }

DEFR(mfhi){ 
    mips_error err = argzerocheck(rs);
	if(err) return err;
    err = argzerocheck(rt);
	if(err) return err;
    err = argzerocheck(shift);
	if(err) return err;
	
	result = state->HI;

	return mips_Success; 
}

DEFR(mflo){ 
    mips_error err = argzerocheck(rs);
	if(err) return err;
    err = argzerocheck(rt);
	if(err) return err;
    err = argzerocheck(shift);
	if(err) return err;
	
	result = state->LO;

	return mips_Success; 
}

DEFR(mthi){ 
    mips_error err = argzerocheck(rt);
	if(err) return err;
    err = argzerocheck(rd);
	if(err) return err;
    err = argzerocheck(shift);
	if(err) return err;
	
	state->HI = src1;
	result = 0;

	return mips_Success; 
}

DEFR(mtlo){ 
    mips_error err = argzerocheck(rt);
	if(err) return err;
    err = argzerocheck(rd);
	if(err) return err;
    err = argzerocheck(shift);
	if(err) return err;
	
	state->LO = src1;
	result = 0;

	return mips_Success; 
}

DEFR(sll){ 
    mips_error err = argzerocheck(rs);
	if(err) return err;

	result = (src2 << shift);
	return mips_Success; 
}

DEFR(srl){ 
    mips_error err = argzerocheck(rs);
	if(err) return err;

	result = (src2 >> shift);
	return mips_Success; 
}

DEFR(sllv){ 
    mips_error err = argzerocheck(shift);
	if(err) return err;
	uint8_t arg = (0x0000001F & src1);

	result = (src2 << arg);
	return mips_Success; 
}

DEFR(srlv){ 
    mips_error err = argzerocheck(shift);
	if(err) return err;
	uint8_t arg = (0x0000001F & src1);

	result = (src2 >> arg);
	return mips_Success; 
}

DEFR(sra){ 
    mips_error err = argzerocheck(rs);
	if(err) return err;

	uint32_t mask = 0x00000000;
	
	if(0x80000000 & src2){
		for(uint8_t i=0; i<shift; i++){
			mask = mask | (0x80000000 >> i);
		}
	}

	result = mask | (src2 >> shift);
	
	return mips_Success; 
}

DEFR(srav){ 
    mips_error err = argzerocheck(shift);
	if(err) return err;
	uint8_t arg = (0x0000001F & src1);

	uint32_t mask = 0x00000000;
	
	if(0x80000000 & src2){
		for(uint8_t i=0; i<arg; i++){
			mask = mask | (0x80000000 >> i);
		}
	}

	result = mask | (src2 >> arg);
	
	return mips_Success; 
}


DEFR(jr){ return mips_ErrorNotImplemented; }
DEFR(jalr){ return mips_ErrorNotImplemented; }

DEFR(slt){ 
	
	mips_error err = argzerocheck(shift);
	if(err) return err;
	
	if(compSigned(src1, src2)) result = 1;
	else result =0;

	return mips_Success; 
}

DEFR(sltu){ 
	
	mips_error err = argzerocheck(shift);
	if(err) return err;
	
	if(src1<src2) result = 1;
	else result =0;
	
	return mips_Success; 
}

/* *********************** I-TYPE *********************** */ 


DEFI(addi){

	// do arithmetics
	result = src1 + imm;
	
	// check overflow
	mips_error err = check_overflow(src1, imm, result);
	if(err) return err;

	return mips_Success;
}


DEFI(addiu){

	// do arithmetics
	result = src1 + imm;
	
    return mips_Success;
}


DEFI(andi){
	// mask top 16bits with 0
	imm = zero_extend(imm);

	// do arithmetics
	result = src1 & imm;

	return mips_Success; 
}

DEFI(ori){
	// mask top 16bits with 0
	imm = zero_extend(imm);

	// do arithmetics
	result = src1 | imm;

	return mips_Success; 
}

DEFI(xori){
	// mask top 16bits with 0
	imm = zero_extend(imm);

	// do arithmetics
	result = src1 ^ imm;

	return mips_Success; 
}


DEFI(b){ 
	if(rd==0x01) return bgez(state, src1, imm, result, rs, rd);
	if(rd==0x11) return bgezal(state, src1, imm, result, rs, rd);
	if(rd==0x00) return bltz(state, src1, imm, result, rs, rd);
	if(rd==0x10) return bltzal(state, src1, imm, result, rs, rd);
	
	return mips_ErrorNotImplemented; 
}

DEFI(bltz){ return mips_ErrorNotImplemented; }
DEFI(bltzal){ return mips_ErrorNotImplemented; }
DEFI(bgez){ return mips_ErrorNotImplemented; }
DEFI(bgezal){ return mips_ErrorNotImplemented; }

DEFI(beq){  
	uint32_t src2;
	mips_cpu_get_register(state, rd, &src2);	// rd is already cut to 5 bits, no error can be received
	
	result=src2;								// make sure you don't change state of cpu after you go back to call

	if(src1 == src2){
		imm = (imm<<2);
		uint32_t nPC, branch;
		mips_cpu_get_npc(state, &nPC);
		
		branch = nPC + imm;
		
		bool immNeg = check_negative(imm);
		bool branchNeg = check_negative(branch);
		bool nPCNeg = check_negative(nPC);
		
		// Negative destination address
		if(immNeg && !nPCNeg && branchNeg) return mips_ExceptionInvalidAddress;
		
		// Destination address overflow
		if(!immNeg && nPCNeg && !branchNeg) return mips_ExceptionInvalidAddress;

		mips_cpu_set_branch(state, imm);	// imm last 2 bits are already zero, no error can be received
	
	}

	return mips_Success;
}

DEFI(bne){ return mips_ErrorNotImplemented; }
DEFI(blez){ return mips_ErrorNotImplemented; }
DEFI(bgtz){ return mips_ErrorNotImplemented; }

DEFI(lw){ return mips_ErrorNotImplemented; }
DEFI(lh){ return mips_ErrorNotImplemented; }
DEFI(lb){ return mips_ErrorNotImplemented; }
DEFI(lwl){ return mips_ErrorNotImplemented; }
DEFI(lwr){ return mips_ErrorNotImplemented; }
DEFI(lhu){ return mips_ErrorNotImplemented; }
DEFI(lbu){ return mips_ErrorNotImplemented; }

DEFI(lui){ return mips_ErrorNotImplemented; }

DEFI(sw){ 
	
	imm = imm + src1;
	if(imm & 0x00000003) return mips_ExceptionInvalidAddress;
	
	uint32_t src2;
	mips_cpu_get_register(state, rd, &src2);	// rd is already cut to 5 bits, no error can be received

	// Load in Memory  
    mips_error err = mips_mem_write(
        steate->mem,                     //!< Handle to target memory
        imm,                         	 //!< Byte address to start transaction at
        4,                               //!< Number of bytes to transfer
        (uint8_t*)&(src2)             	 //!< Receives the target bytes
    );

    result = src2;
    return mips_Success; 
}


DEFI(sh){ return mips_ErrorNotImplemented; }
DEFI(sb){ return mips_ErrorNotImplemented; }

DEFI(slti){ 
	
	if(compSigned(src1, imm)) result = 1;
	else result =0;

	return mips_Success; 
}

DEFI(sltiu){ 
	
	if(src1<imm) result = 1;
	else result =0;
	
	return mips_Success; 
}

/* *********************** J-TYPE *********************** */ 

DEFJ(j){ return mips_ErrorNotImplemented; }
DEFJ(jal){ return mips_ErrorNotImplemented; }
