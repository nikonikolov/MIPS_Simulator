#ifndef mips_cpu_decode_h
#define mips_cpu_decode_h

#include "mips.h"		// required for var types
#include "common_print.h"
#include "Ins/Ins.h"
#include "InsI/InsI.h"
#include "InsR/InsR.h"
#include "InsJ/InsJ.h"
#include "mips_cpu_ins_set.h"
#include "mips_cpu_private.h"

#include <vector>

using namespace std;

#define R 0
#define I 1
#define J 2

typedef mips_error (*FP)(mips_cpu_h, uint32_t);

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


FP decodeType(uint32_t InsWord);

mips_error decodeR(mips_cpu_h state, uint32_t InsWord);
mips_error decodeI(mips_cpu_h state, uint32_t InsWord);
mips_error decodeJ(mips_cpu_h state, uint32_t InsWord);


#endif
