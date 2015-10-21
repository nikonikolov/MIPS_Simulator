#ifndef mips_cpu_ins_decode_h
#define mips_cpu_ins_decode_h

#include "mips.h"		// required for var types
#include <vector>
#include "Ins/Ins.h"
#include "InsI/InsI.h"
#include "InsR/InsR.h"
#include "InsJ/InsJ.h"
#include "mips_cpu_ins_set.h"

using namespace std;

#define R 0
#define I 1
#define J 2

typedef mips_error (*FP)(mips_cpu_h, uint32_t);
typedef InsR<FPR> InsObjR;

extern const vector<InsObjR> PtrR;
extern const vector<InsObjI> PtrI;
extern const vector<InsObjJ> PtrJ;


template <class InsObjType>
const InsObjType* findIns(uint8_t opcode, const vector<InsObjType>& InsStruct){
	for(int i=0; i<InsStruct.size(); i++){
		if(opcode == InsStruct[i].get_opcode() ){
			return &(InsStruct[i]);
		}
	}
	return NULL;
}

/*template <class FnPtrType, class ObjType>
void findIns(uint8_t opcode, const vector<ObjType>& InsStruct, FnPtrType& FnImpl){
	for(int i=0; i<InsStruct.size(); i++){
		if(opcode == InsStruct[i].get_opcode() ){
			FnImpl =InsStruct[i].get_FnImpl();
			return ;
		}
	}
	FnImpl = NULL;
}*/

FP decodeType(uint32_t InsWord);
FP insType(uint8_t instype);


mips_error decodeR(mips_cpu_h state, uint32_t InsWord);
mips_error decodeI(mips_cpu_h state, uint32_t InsWord);
mips_error decodeJ(mips_cpu_h state, uint32_t InsWord);



uint8_t extr_opcode(uint32_t InsWord);
uint8_t extr_src1(uint32_t InsWord);
uint8_t extr_src2(uint32_t InsWord);
uint8_t extr_dest(uint32_t InsWord);
uint8_t extr_shift(uint32_t InsWord);
uint8_t extr_fn(uint32_t InsWord);
uint16_t extr_imm(uint32_t InsWord);


mips_error extr_R(mips_cpu_h state, uint32_t InsWord, uint32_t& src1, uint32_t& src2, uint8_t& rd, uint8_t& shift);



#endif
