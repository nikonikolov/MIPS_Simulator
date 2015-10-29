#ifndef test_mips_print_H
#define test_mips_print_H

#include "mips.h"
#include "mips_cpu_def.h"
#include "mips_cpu_private.h"


mips_error printPC(mips_cpu_h state, FILE *dest);

void debugPrintWord(FILE *dest, const uint32_t& InsWord, char* id =NULL);

mips_error printErr(FILE *dest, mips_error err, char* msg);

#endif