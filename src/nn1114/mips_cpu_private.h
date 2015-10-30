
/* Header for CPU internal functionality */ 

#ifndef mips_private_h
#define mips_private_h

#include "mips.h"
#include "mips_cpu_def.h"


using namespace std;

/* ************************* CPU RELATED *************************** */

uint32_t nn1114_change_endian(uint32_t word);

mips_error advance_pc (mips_cpu_h state, uint32_t offset);

mips_error check_overflow(const uint32_t& src1, const uint32_t& src2, const uint32_t& result);

mips_error argzerocheck(const uint8_t& shift);

bool check_negative(const uint32_t& arg);

mips_error nn1114_mips_cpu_get_npc(
	mips_cpu_h state,	//!< Valid (non-empty) handle to a CPU
	uint32_t *npc		//!< Where to write the byte address too
);

mips_error nn1114_mips_cpu_set_npc(
	mips_cpu_h state,		//!< Valid (non-empty) handle to a CPU
	uint32_t npc			//!< Address of the next instruction to exectute.
);	

mips_error nn1114_mips_cpu_set_branch(
	mips_cpu_h state,		//!< Valid (non-empty) handle to a CPU
	uint32_t branch			//!< Address of the next instruction to exectute.
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
uint32_t sign_extend(uint16_t arg);


/* ************************* DEBUGGING *************************** */

/*void debugPrintWord(mips_cpu_h state, const uint32_t& InsWord, char* id =NULL);
mips_error printPC(mips_cpu_h state);
mips_error printErr(mips_cpu_h state, mips_error err, char* msg);
*/

#endif