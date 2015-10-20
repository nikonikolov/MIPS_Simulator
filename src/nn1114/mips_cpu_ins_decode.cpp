#include "mips_cpu_ins_decode.h"

// R-type instuctions
const vector<Ins> PtrR = { 	Ins(0x02 /*000001*/, &j), 
							Ins(0x03/*000011*/, &jal) 
						 };

// I-type instructions
const vector<Ins> PtrI = { 	Ins(0x02 /*000001*/, &j), 
							Ins(0x03/*000011*/, &jal) 
						 };

// J-type instructions
const vector<Ins> PtrJ = { 	Ins(0x02 /*000001*/, &j), 
							Ins(0x03/*000011*/, &jal) 
						 };


// Hash Table
const vector<Ins>* HashT[3]={&PtrR, &PtrI, &PtrJ};


// Decode Instruction Word and call the corresponding function to execute instruction
mips_error decode(mips_cpu_h state, uint32_t InsWord){
	
	// extract opcode
	uint8_t opcode = extr_opcode(InsWord);

	// find type of InsWord in Hash Table 
	int idx = hashfn(opcode);

	//declare function pointer
	FP FnImpl;
	
	// find InsWord in vector inside hash table
	if(idx==R) 			FnImpl = findIns( extr_R_fn(InsWord), HashT[idx]);
	else if(idx==I || idx==J) 	FnImpl = findIns(opcode, HashT[idx]);
	else return mips_ErrorNotImplemented;	// Is this the best error code

	// Error if InsWord is not found
	if(FnImpl == NULL) return mips_ErrorNotImplemented;

	// Execute instruction
	FnImpl(state, InsWord);

	return mips_Success;
}


// Return pointer to an implementation of a function 
FP findIns(uint8_t opcode, const vector<Ins>*& InsStruct){
	for(int i=0; i<InsStruct->size(); i++){
		if(opcode == (*InsStruct)[i].get_opcode() ) return (*InsStruct)[i].get_FnImpl();
	}
	return NULL;
}


// Extract opcode of instruction word
uint8_t extr_opcode(uint32_t InsWord){
	return (InsWord>>26) & 0x3F;
}


// Extract function type of R-type instruction 
uint8_t extr_R_fn(uint32_t InsWord){
	return InsWord & 0x0000003F;
}


// Hash function
int hashfn(uint8_t instype){
	if (instype == 0x00) return R;
	else if ( ((instype>>1) & 0x1F) == 0x01 ) return J;
	else return I; 
}


