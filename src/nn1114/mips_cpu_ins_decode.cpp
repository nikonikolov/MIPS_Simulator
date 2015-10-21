#include "mips_cpu_ins_decode.h"

// R-type instuctions
const vector<InsObjR> PtrR = {	InsObjR(0x20 /*100000*/, "add", &add), 
								InsObjR(0x21 /*100001*/, "addu", &addu) 
						 	 };

// I-type instructions
const vector<InsObjI> PtrI = { 	InsObjI(0x08 /*001000*/, "addi", &addi), 
								InsObjI(0x09 /*001001*/, "addiu", &addiu) 
						 	 };

// J-type instructions
const vector<InsObjJ> PtrJ = { 	InsObjJ(0x02 /*000001*/, "j", &j), 
								InsObjJ(0x03 /*000011*/, "jal", &jal) 
						 	 };


// Decode Instruction Word and call the corresponding function to execute instruction
FP decodeType(uint32_t InsWord){
	
	// extract opcode
	uint8_t opcode = extr_opcode(InsWord);

	// find type of InsWord
	return insType(opcode);
}


// Determine instruction type function
FP insType(uint8_t instype){
	if (instype == 0x00) return &decodeR;
	else if ( ((instype>>1) & 0x1F) == 0x01 ) return &decodeJ;
	else return &decodeI; 
}

// Extract opcode of instruction word
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


mips_error extr_R(mips_cpu_h state, uint32_t InsWord, uint32_t& src1, uint32_t& src2, uint8_t& rd, uint8_t& shift){

	uint8_t rs, rt;

	rs = extr_src1(InsWord);
	rt = extr_src2(InsWord);
	rd = extr_dest(InsWord);
	shift = extr_shift(InsWord);

	mips_error err = mips_cpu_get_register(state, rs, &src1);
	if(!err) return err;
	return mips_cpu_get_register(state, rt, &src2); 
}


mips_error decodeR(mips_cpu_h state, uint32_t InsWord){

	// find instruction
	const InsObjR *InsObj = findIns( extr_fn(InsWord), PtrR);
	if(InsObj==NULL) return mips_ErrorNotImplemented;
	
	FPR FnImpl = InsObj->get_FnImpl();

	// decode instruction
	uint32_t src1, src2;
	uint8_t rd, shift;
	extr_R(state, InsWord, src1, src2, rd, shift);

	// declare var to store result from instruction
	uint64_t result;

	// execute instruction
	mips_error err = FnImpl(src1, src2, result, shift);
	if(!err) return err;

	// write to register
	return mips_cpu_set_register(state, rd, (uint32_t)result);
}


mips_error extr_I(mips_cpu_h state, uint32_t InsWord, uint32_t& src1, uint16_t& imm, uint8_t& rd){

	uint8_t rs;

	rs = extr_src1(InsWord);
	rd = extr_dest(InsWord);
	imm = extr_imm(InsWord);

	return mips_cpu_get_register(state, rs, &src1);
}


mips_error decodeI(mips_cpu_h state, uint32_t InsWord){

	// find instruction
	const InsObjI *InsObj = findIns( extr_opcode(InsWord), PtrI);
	if(InsObj==NULL) return mips_ErrorNotImplemented;
	
	FPI FnImpl = InsObj->get_FnImpl();

	// decode instruction
	uint32_t src1;
	uint8_t rd;
	uint16_t imm;
	extr_I(state, InsWord, src1, imm, rd);

	// declare var to store result from instruction
	uint64_t result;

	// execute instruction
	mips_error err = FnImpl(src1, imm, result);
	if(!err) return err;

	// write to register
	return mips_cpu_set_register(state, rd, (uint32_t)result);
}


mips_error decodeJ(mips_cpu_h state, uint32_t InsWord){
	
	// find instruction
	const InsObjJ *InsObj = findIns( extr_opcode(InsWord), PtrJ);
	if(InsObj==NULL) return mips_ErrorNotImplemented;
	
	FPJ FnImpl = InsObj->get_FnImpl();

	return mips_Success;
}