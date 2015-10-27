#include "mips_cpu_decode.h"

// R-type instuctions
const vector<InsObjR> PtrR = {	InsObjR(0x20 /*100000*/, "add", 	&add), 
								InsObjR(0x21 /*100001*/, "addu", 	&addu), 
								InsObjR(0x22 /*100010*/, "sub", 	&sub), 
								InsObjR(0x23 /*100011*/, "subu", 	&subu), 
								
								InsObjR(0x24 /*100100*/, "and", 	&andr), 
								InsObjR(0x25 /*100101*/, "or",		&orr), 
								InsObjR(0x26 /*100110*/, "xor",		&xorr), 
								
								InsObjR(0x1A /*011010*/, "div", 	&divr), 
								InsObjR(0x1B /*011011*/, "divu", 	&divu), 
								InsObjR(0x28 /*011000*/, "mult",	&mult), 
								InsObjR(0x29 /*011001*/, "multu",	&multu), 

								InsObjR(0x10 /*010000*/, "mfhi", 	&mfhi), 
								InsObjR(0x12 /*010010*/, "mflo", 	&mflo), 
								InsObjR(0x11 /*010001*/, "mthi", 	&mthi), 
								InsObjR(0x13 /*010011*/, "mtlo", 	&mtlo), 
								
								InsObjR(0x00 /*000000*/, "sll",		&sll), 
								InsObjR(0x02 /*000010*/, "srl",		&srl), 
								InsObjR(0x04 /*000100*/, "sllv",	&sllv), 
								InsObjR(0x06 /*000110*/, "srlv",	&srlv), 
								InsObjR(0x03 /*000011*/, "sra",		&sra), 
								InsObjR(0x07 /*000111*/, "srav",	&srav), 

								InsObjR(0x09 /*001001*/, "jalr", 	&jalr), 
								InsObjR(0x08 /*001000*/, "jr", 		&jr), 

								InsObjR(0x2A /*101010*/, "slt",		&slt), 
								InsObjR(0x2B /*101011*/, "sltu",	&sltu) 
						 	 };

// I-type instructions
const vector<InsObjI> PtrI = { 	InsObjI(0x08 /*001000*/, "addi", 	&addi), 
								InsObjI(0x09 /*001001*/, "addiu", 	&addiu),

								InsObjI(0x0C /*001100*/, "andi", 	&andi), 
								InsObjI(0x0D /*001101*/, "ori", 	&ori), 
								InsObjI(0x0E /*001110*/, "xori", 	&xori), 

								// NOTE: YOU NEED TO PROCESS BLTZ,BLTZAL, BGEZ, BGEZAL INSIDE THIS ONE AS WELL
								InsObjI(0x01 /*000001*/, "b", 		&b),
								InsObjI(0x04 /*000100*/, "beq", 	&beq), 
								InsObjI(0x05 /*000101*/, "bne", 	&bne), 								 		
								InsObjI(0x06 /*000110*/, "blez", 	&blez), 
								InsObjI(0x07 /*000111*/, "bgtz", 	&bgtz), 

								InsObjI(0x23 /*100011*/, "lw",		&lw), 
								InsObjI(0x21 /*100001*/, "lh",		&lh), 
								InsObjI(0x20 /*100000*/, "lb",		&lb), 
								InsObjI(0x22 /*100010*/, "lwl",		&lwl), 
								InsObjI(0x26 /*100110*/, "lwr",		&lwr), 
								InsObjI(0x25 /*100101*/, "lhu",		&lhu), 
								InsObjI(0x24 /*100100*/, "lbu",		&lbu), 
								
								InsObjI(0x0F /*001111*/, "lui",		&lui), 
								
								InsObjI(0x2A /*101011*/, "sw",		&sw), 
								InsObjI(0x29 /*101001*/, "sh",		&sh), 
								InsObjI(0x28 /*101000*/, "sb",		&sb), 
								
								InsObjI(0x0A /*001010*/, "slti",	&slti), 
								InsObjI(0x0B /*001011*/, "sltiu",	&sltiu) 
						 	 };

// J-type instructions
const vector<InsObjJ> PtrJ = { 	InsObjJ(0x02 /*000001*/, "j", &j), 
								InsObjJ(0x03 /*000011*/, "jal", &jal) 
						 	 };


/* *********************** HELP FUNCTIONS NEEDED ONLY IN THE FUNCTIONS IN THE CORRESPONDING HEADER *********************** */ 


// Determine instruction type function
static FP insType(uint8_t instype){
	if (instype == 0x00) return &decodeR;
	else if ( ((instype>>1) & 0x1F) == 0x01 ) return &decodeJ;
	else return &decodeI; 
}


static mips_error extr_R(mips_cpu_h state, uint32_t InsWord, uint32_t& src1, uint32_t& src2, uint8_t& rd, uint8_t& shift){

	uint8_t rs, rt;

	rs = extr_src1(InsWord);
	rt = extr_src2(InsWord);
	rd = extr_dest(InsWord);
	shift = extr_shift(InsWord);

	mips_error err = mips_cpu_get_register(state, rs, &src1);
	if(err) return printErr(state, err, "Fn: extr_R, get register rs unsuccessful");	

	err = mips_cpu_get_register(state, rt, &src2);
	if(err) return printErr(state, err, "Fn: extr_R, get register rt unsuccessful");	

	return mips_Success; 
}


static mips_error extr_I(mips_cpu_h state, uint32_t InsWord, uint32_t& src1, uint16_t& imm, uint8_t& rd){

	uint8_t rs;

	rs = extr_src1(InsWord);
	rd = extr_src2(InsWord);
	imm = extr_imm(InsWord);

	mips_error err = mips_cpu_get_register(state, rs, &src1);
	if(err) return printErr(state, err, "Fn: extr_I, get register rs unsuccessful");	

	return mips_Success; 
}


static mips_error extr_J(uint32_t InsWord, uint32_t& arg){

	arg = extr_jarg(InsWord);
	return mips_Success;
}

/* *********************** END HELP FUNCTIONS NEEDED ONLY IN THE FUNCTIONS IN THE CORRESPONDING HEADER *********************** */ 


// Decode Instruction Word and call the corresponding function to execute instruction
FP decodeType(uint32_t InsWord){
	
	// extract opcode
	uint8_t opcode = extr_opcode(InsWord);

	// find type of InsWord
	return insType(opcode);
}


/* *********************** DECODE DIFFERENT TYPES OF INSTRUCTIONS *********************** */ 


mips_error decodeR(mips_cpu_h state, uint32_t InsWord){

	// find instruction
	const InsObjR *InsObj = findIns( extr_fn(InsWord), PtrR);
	if(InsObj==NULL)  return printErr(state, mips_ErrorNotImplemented, "Fn: decodeR, findIns unsuccessful");	


	FPR FnImpl = InsObj->get_FnImpl();

	// decode instruction
	uint32_t src1, src2;
	uint8_t rd, shift;
	extr_R(state, InsWord, src1, src2, rd, shift);

	// declare var to store result from instruction
	uint32_t result;

	// execute instruction
	mips_error err = FnImpl(src1, src2, result, shift);
	if(err) return printErr(state, err, "Fn: decodeR, execute(FnImpl) unsuccessful");	

	// print if logLevel is set
	err = InsObj->debugIns(state, InsWord, result);
	
	// write to register
	return mips_cpu_set_register(state, rd, result);
}


mips_error decodeI(mips_cpu_h state, uint32_t InsWord){

	// find instruction
	const InsObjI *InsObj = findIns( extr_opcode(InsWord), PtrI);
	if(InsObj==NULL)  return printErr(state, mips_ErrorNotImplemented, "Fn: decodeI, findIns unsuccessful");	

	
	FPI FnImpl = InsObj->get_FnImpl();

	// decode instruction
	uint32_t src1;
	uint8_t rd;
	uint16_t imm;
	extr_I(state, InsWord, src1, imm, rd);

	// declare var to store result from instruction
	uint32_t result;

	// execute instruction
	mips_error err = FnImpl(src1, sign_extend(imm), result);
	if(err) return printErr(state, err, "Fn: decodeI, execute(FnImpl) unsuccessful");	

	// print if logLevel is set
	err = InsObj->debugIns(state, InsWord, result);

	// write to register
	return mips_cpu_set_register(state, rd, result);
}


mips_error decodeJ(mips_cpu_h state, uint32_t InsWord){

	
	// find instruction
	const InsObjJ *InsObj = findIns( extr_opcode(InsWord), PtrJ);
	if(InsObj==NULL)  return printErr(state, mips_ErrorNotImplemented, "Fn: decodeJ, findIns unsuccessful");	
	
	FPJ FnImpl = InsObj->get_FnImpl();

	uint32_t arg;
	extr_J(InsWord, arg);
	
	// declare var to store result from instruction
	uint32_t result;

	// execute instruction
	mips_error err = FnImpl(state, arg);
	if(err) return printErr(state, err, "Fn: decodeJ, execute(FnImpl) unsuccessful");	

	// print if logLevel is set
	err = InsObj->debugIns(state, InsWord, result);

	return mips_Success;
}