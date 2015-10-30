#ifndef mips_cpu_ins_set_h
#define mips_cpu_ins_set_h

#include "mips.h"
#include "mips_cpu_decode.h"
#include "mips_cpu_private.h"
#include "mips_cpu_print.h"
#include <cmath>

#define DEFR(X) mips_error X(mips_cpu_h state, uint32_t src1, uint32_t src2, uint32_t& result, uint8_t shift, uint8_t rs, uint8_t rt, uint8_t rd, bool& write)

#define CALLR(X) X(state, src1, src2, result, shift, rs, rt, rd, write)

#define DEFI(X) mips_error X(mips_cpu_h state, uint32_t src1, uint32_t imm, uint32_t& result, uint8_t rs, uint8_t rd, bool& write)

#define CALLI(X) X(state, src1, imm, result, rs, rd, write)

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


//#undef DEFR
//#undef DEFI
//#undef DEFJ

#endif