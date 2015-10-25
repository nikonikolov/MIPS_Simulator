#include "mips_cpu_ins_set.h"

// R-type instructions
mips_error add(uint32_t src1, uint32_t src2, uint32_t& result, uint8_t shift){
	
	mips_error err = shift_check(shift);
	if(err) return err;
	
	// do arithmetics
	result = src1 + src2;
	
	// check overflow
	err = check_overflow(src1, src2, result);
	if(err) return err;

	return mips_Success;
}


mips_error addu(uint32_t src1, uint32_t src2, uint32_t& result, uint8_t shift){

    mips_error err = shift_check(shift);
	if(err) return err;

	// do arithmetics
	result = src1 + src2;
	
	return mips_Success;
}

// I-type instructions
mips_error addi(uint32_t src1, uint32_t imm, uint32_t& result){

	// do arithmetics
	result = src1 + imm;
	
	// check overflow
	mips_error err = check_overflow(src1, imm, result);
	if(err) return err;

	return mips_Success;
}


mips_error addiu(uint32_t src1, uint32_t imm, uint32_t& result){

	// do arithmetics
	result = src1 + imm;
	
    return mips_Success;
}


// J-type instructions
mips_error jal(mips_cpu_h state, uint32_t InsWord){
    return mips_ErrorNotImplemented;
}

mips_error j(mips_cpu_h state, uint32_t InsWord){
    return mips_ErrorNotImplemented;
}


