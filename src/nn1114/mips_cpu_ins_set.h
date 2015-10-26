#ifndef mips_cpu_ins_set_h
#define mips_cpu_ins_set_h

#include "mips.h"
#include "mips_cpu_decode.h"
#include "mips_cpu_private.h"
#include "common_print.h"

#define DEFR(X) mips_error X(uint32_t src1, uint32_t src2, uint32_t& result, uint8_t shift)

#define DEFI(X) mips_error X(uint32_t src1, uint32_t imm, uint32_t& result)

#define DEFJ(X) mips_error X(mips_cpu_h state, uint32_t InsWord)


/* *********************** R-TYPE *********************** */ 


DEFR(add);
DEFR(addu);
DEFR(sub);
DEFR(subu);

DEFR(andr);
DEFR(orr);
DEFR(xorr);

DEFR(divr);
DEFR(divu);
DEFR(mult);
DEFR(multu);

DEFR(mfhi);
DEFR(mflo);
DEFR(mthi);
DEFR(mtlo);

DEFR(sll);
DEFR(srl);
DEFR(sllv);
DEFR(srlv);
DEFR(sra);
DEFR(srav);

DEFR(jr);
DEFR(jalr);

DEFR(slt);
DEFR(sltu);


//mips_error add(uint32_t src1, uint32_t src2, uint32_t& result, uint8_t shift);
//mips_error addu(uint32_t src1, uint32_t src2, uint32_t& result, uint8_t shift);

//mips_error addi(uint32_t src1, uint32_t imm, uint32_t& result);
//mips_error addiu(uint32_t src1, uint32_t imm, uint32_t& result);

/* *********************** I-TYPE *********************** */ 


DEFI(addi);
DEFI(addiu);

DEFI(andi);
DEFI(ori);
DEFI(xori);

DEFI(b);
DEFI(bltz);
DEFI(bltzal);
DEFI(bgez);
DEFI(bgezal);
DEFI(beq);
DEFI(bne);
DEFI(blez);
DEFI(bgtz);

DEFI(lw);
DEFI(lh);
DEFI(lb);
DEFI(lwl);
DEFI(lwr);
DEFI(lhu);
DEFI(lbu);

DEFI(lui);

DEFI(sw);
DEFI(sh);
DEFI(sb);

DEFI(slti);
DEFI(sltiu);


/* *********************** J-TYPE *********************** */ 

DEFJ(j);
DEFJ(jal);

//mips_error jal(mips_cpu_h state, uint32_t InsWord);
//mips_error j(mips_cpu_h state, uint32_t InsWord);


//#undef DEFR
//#undef DEFI
//#undef DEFJ

#endif