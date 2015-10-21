
/* Header for CPU internal functionality */ 

#ifndef mips_private_h
#define mips_private_h

#include "mips.h"	// required for var types
#include "mips_cpu_def.h"

using namespace std;

/* ************************* CPU RELATED *************************** */

uint32_t change_endian(uint32_t word);

mips_error advance_pc (mips_cpu_h state, uint32_t offset);

mips_error check_overflow(const uint64_t& result);

mips_error shift_check(const uint8_t& shift);

mips_error mips_cpu_get_npc(
	mips_cpu_h state,	//!< Valid (non-empty) handle to a CPU
	uint32_t *npc		//!< Where to write the byte address too
);

/* ************************* WORD EXTRACTION *************************** */

uint8_t extr_opcode(uint32_t InsWord);
uint8_t extr_src1(uint32_t InsWord);
uint8_t extr_src2(uint32_t InsWord);
uint8_t extr_dest(uint32_t InsWord);
uint8_t extr_shift(uint32_t InsWord);
uint8_t extr_fn(uint32_t InsWord);
uint16_t extr_imm(uint32_t InsWord);
uint32_t extr_jarg(uint32_t InsWord);


/* ************************* DEBUGGING *************************** */

void debugPrintWord(mips_cpu_h state, const uint32_t& InsWord);

void printBin(mips_cpu_h state, uint32_t InsWord);

mips_error printPC(mips_cpu_h state);


/*void debugPrintInsR(mips_cpu_h state, uint32_t InsWord, ){

	if(state->logLevel == LOGMIDDLE){
        fprintf(state->logDst, "R-Type : dst=%u, src1=%u, src2=%u, shift=%u, function=%u.\n  instr=%08x\n",
            							dst, src1, src2, shift, function, instruction
        );
    }

    else if (state->logLevel == LOGHIGH){
    	debugPrintWord(state, InsWord);
    }
}

if(state->logLevel >= 1){
                fprintf(state->logDst, "addu %u, %u, %u.\n", dst, src1, src2);
            }*/



#endif