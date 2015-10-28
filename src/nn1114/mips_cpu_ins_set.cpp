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

DEFR(mfhi){ return mips_ErrorNotImplemented; }
DEFR(mflo){ return mips_ErrorNotImplemented; }
DEFR(mthi){ return mips_ErrorNotImplemented; }
DEFR(mtlo){ return mips_ErrorNotImplemented; }

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


DEFI(b){ return mips_ErrorNotImplemented; }
DEFI(bltz){ return mips_ErrorNotImplemented; }
DEFI(bltzal){ return mips_ErrorNotImplemented; }
DEFI(bgez){ return mips_ErrorNotImplemented; }
DEFI(bgezal){ return mips_ErrorNotImplemented; }
DEFI(beq){ return mips_ErrorNotImplemented; }
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

DEFI(sw){ return mips_ErrorNotImplemented; }
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
