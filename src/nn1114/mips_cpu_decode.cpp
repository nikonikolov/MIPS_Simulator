#include "mips_cpu_decode.h"

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
	rd = extr_dest(InsWord);
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
	return mips_cpu_set_register(state, rd, (uint32_t)result);
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
	return mips_cpu_set_register(state, rd, (uint32_t)result);
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