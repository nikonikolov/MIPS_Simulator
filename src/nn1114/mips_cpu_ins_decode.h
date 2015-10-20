#ifndef mips_cpu_ins_decode_h
#define mips_cpu_ins_decode_h

#include "mips.h"		// required for var types
#include <vector>
#include "Ins/Ins.h"
#include "mips_cpu_ins_set.h"

using namespace std;

#define R 0
#define I 1
#define J 2


extern const vector<Ins>* HashT[3];
extern const vector<Ins> PtrR;
extern const vector<Ins> PtrI;
extern const vector<Ins> PtrJ;

FP findIns(uint8_t opcode, const vector<Ins>*& InsStruct);

int hashfn(uint8_t instype);

mips_error decode(mips_cpu_h state, uint32_t InsWord);

uint8_t extr_R_fn(uint32_t InsWord);
uint8_t extr_opcode(uint32_t InsWord);

#endif
