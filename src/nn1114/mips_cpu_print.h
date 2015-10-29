#ifndef mips_cpu_print_H
#define mips_cpu_print_H

#include "mips.h"
#include "mips_cpu_def.h"
#include "mips_cpu_private.h"


void debugPrintWord(mips_cpu_h state, const uint32_t& InsWord, char* id =NULL);
mips_error printPC(mips_cpu_h state);
mips_error printErr(mips_cpu_h state, mips_error err, char* msg);


#endif