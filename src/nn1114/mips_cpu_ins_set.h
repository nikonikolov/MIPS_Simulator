#ifndef mips_cpu_ins_set_h
#define mips_cpu_ins_set_h

#include "mips.h"
#include "mips_cpu_decode.h"
#include "mips_cpu_private.h"
#include "common_print.h"


mips_error add(uint32_t src1, uint32_t src2, uint32_t& result, uint8_t shift);
mips_error addu(uint32_t src1, uint32_t src2, uint32_t& result, uint8_t shift);

mips_error addi(uint32_t src1, uint32_t imm, uint32_t& result);
mips_error addiu(uint32_t src1, uint32_t imm, uint32_t& result);


mips_error jal(mips_cpu_h state, uint32_t InsWord);
mips_error j(mips_cpu_h state, uint32_t InsWord);

#endif