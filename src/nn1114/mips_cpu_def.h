#ifndef mips_definition
#define mips_definition

#include "mips.h"

#define BLOCKSIZE 	4
#define REGS 		32

#define LOGLOW 		1
#define LOGMIDDLE 	2
#define LOGHIGH 	3
#define LOGERROR	1

struct mips_cpu_impl{

	uint32_t PC;
	uint32_t nPC;
	uint32_t branch; // branch offset

	uint32_t regs[REGS];
	
	uint32_t HI;
	uint32_t LO;

	mips_mem_h memPtr;	

	unsigned logLevel;
    FILE *logDst;
};

#endif