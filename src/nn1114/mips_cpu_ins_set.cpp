#include "mips_cpu_ins_set.h"


DEFR(add){
	
	mips_error err = shift_check(shift);
	if(err) return err;
	
	// do arithmetics
	result = src1 + src2;
	
	// check overflow
	err = check_overflow(src1, src2, result);
	if(err) return err;

	return mips_Success;
}


DEFR(addu){

    mips_error err = shift_check(shift);
	if(err) return err;

	// do arithmetics
	result = src1 + src2;
	
	return mips_Success;
}


DEFR(sub){ return mips_ErrorNotImplemented; }
DEFR(subu){ return mips_ErrorNotImplemented; }

DEFR(andr){ return mips_ErrorNotImplemented; }
DEFR(orr){ return mips_ErrorNotImplemented; }
DEFR(xorr){ return mips_ErrorNotImplemented; }

DEFR(divr){ return mips_ErrorNotImplemented; }
DEFR(divu){ return mips_ErrorNotImplemented; }
DEFR(mult){ return mips_ErrorNotImplemented; }
DEFR(multu){ return mips_ErrorNotImplemented; }

DEFR(mfhi){ return mips_ErrorNotImplemented; }
DEFR(mflo){ return mips_ErrorNotImplemented; }
DEFR(mthi){ return mips_ErrorNotImplemented; }
DEFR(mtlo){ return mips_ErrorNotImplemented; }

DEFR(sll){ return mips_ErrorNotImplemented; }
DEFR(srl){ return mips_ErrorNotImplemented; }
DEFR(sllv){ return mips_ErrorNotImplemented; }
DEFR(srlv){ return mips_ErrorNotImplemented; }
DEFR(sra){ return mips_ErrorNotImplemented; }
DEFR(srav){ return mips_ErrorNotImplemented; }

DEFR(jr){ return mips_ErrorNotImplemented; }
DEFR(jalr){ return mips_ErrorNotImplemented; }

DEFR(slt){ return mips_ErrorNotImplemented; }
DEFR(sltu){ return mips_ErrorNotImplemented; }


/* *********************** I-TYPE *********************** */ 


DEFI(addi){

	// do arithmetics
	result = src1 + imm;
	
	// check overflow
	mips_error err = check_overflow(src1, imm, result);
	if(err) return err;

	return mips_Success;
}


DEFI(addiu){

	// do arithmetics
	result = src1 + imm;
	
    return mips_Success;
}


DEFI(andi){ return mips_ErrorNotImplemented; }
DEFI(ori){ return mips_ErrorNotImplemented; }
DEFI(xori){ return mips_ErrorNotImplemented; }

DEFI(b){ return mips_ErrorNotImplemented; }
DEFI(bltz){ return mips_ErrorNotImplemented; }
DEFI(bltzal){ return mips_ErrorNotImplemented; }
DEFI(bgez){ return mips_ErrorNotImplemented; }
DEFI(bgezal){ return mips_ErrorNotImplemented; }
DEFI(beq){ return mips_ErrorNotImplemented; }
DEFI(bne){ return mips_ErrorNotImplemented; }
DEFI(blez){ return mips_ErrorNotImplemented; }
DEFI(bgtz){ return mips_ErrorNotImplemented; }

DEFI(lw){ return mips_ErrorNotImplemented; }
DEFI(lh){ return mips_ErrorNotImplemented; }
DEFI(lb){ return mips_ErrorNotImplemented; }
DEFI(lwl){ return mips_ErrorNotImplemented; }
DEFI(lwr){ return mips_ErrorNotImplemented; }
DEFI(lhu){ return mips_ErrorNotImplemented; }
DEFI(lbu){ return mips_ErrorNotImplemented; }

DEFI(lui){ return mips_ErrorNotImplemented; }

DEFI(sw){ return mips_ErrorNotImplemented; }
DEFI(sh){ return mips_ErrorNotImplemented; }
DEFI(sb){ return mips_ErrorNotImplemented; }

DEFI(slti){ return mips_ErrorNotImplemented; }
DEFI(sltiu){ return mips_ErrorNotImplemented; }


/* *********************** J-TYPE *********************** */ 

DEFJ(j){ return mips_ErrorNotImplemented; }
DEFJ(jal){ return mips_ErrorNotImplemented; }
