#ifndef mips_cpu_ins_set_h
#define mips_cpu_ins_set_h

#include "mips.h"
#include "mips_cpu_ins_decode.h"
#include "mips_cpu_private.h"

mips_error add(uint32_t src1, uint32_t src2, uint64_t& result, uint8_t shift);
mips_error addu(uint32_t src1, uint32_t src2, uint64_t& result, uint8_t shift);

mips_error addi(uint32_t src1, uint16_t imm, uint64_t& result);
mips_error addiu(uint32_t src1, uint16_t imm, uint64_t& result);


mips_error jal(uint32_t InsWord);
mips_error j(uint32_t InsWord);

#endif