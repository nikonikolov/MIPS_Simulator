#include "InsRCSV.h"

InsRCSV::InsRCSV(string nameIn, uint8_t opcodeIn /*=0*/, uint8_t rsIn, uint8_t rtIn, uint8_t rdIn, uint8_t shiftIn /*=0*/, uint8_t fnIn,
			 uint32_t src1In, uint32_t src2In, uint32_t resultIn, string msgIn)	:
			
			InsCSV(nameIn, opcodeIn, msgIn), rs(rsIn), rt(rtIn), rd(rdIn), shift(shiftIn), fn(fnIn),
			src1(src1In), src2(src2In), result(resultIn) {}


uint32_t InsRCSV::Build(){
    return
        (InsCSV::opcode << 26)  |   // opcode = 0
        (rs << 21)      |   		// reg 1
        (rt << 16)      |   		// reg 2
        (rd << 11)      |   		// dest reg
        (shift << 6)    |   		// shift
        (fn << 0);          		// fn code
}


void InsRCSV::SetRegs(mips_cpu_h cpuPtr){

    // NOTE : C STORES NEGATIVE NUMBERS INSIDE UINT TYPE WITH THEIR TWO'S COMLEMENT EQUIVALENT, SO NO ADDITIONAL CHECKS ARE REQUIRED
	mips_error err = mips_cpu_set_register(cpuPtr, rs, src1);
    checkRegSet(err);
    err = mips_cpu_set_register(cpuPtr, rt, src2);
    checkRegSet(err);
}
    

int InsRCSV::CheckResult(mips_cpu_h cpuPtr, char** msg){
    uint32_t calcResult;

    mips_error err = mips_cpu_get_register(cpuPtr, rd, &calcResult);
    checkRegGet(err);

    // Modify message
    *msg = InsCSV::get_msg();

    // NOTE : C STORES NEGATIVE NUMBERS INSIDE UINT TYPE WITH THEIR TWO'S COMLEMENT EQUIVALENT, SO NO ADDITIONAL CHECKS ARE REQUIRED
    return result == calcResult;
}

void InsRCSV::printInsObj(mips_cpu_h state){
    fprintf(state->logDst, "InsRCSV Object values: ");
    fprintf(state->logDst, "name: %s ", InsCSV::get_name());
    fprintf(state->logDst, "opcode: %x ", InsCSV::opcode);
    fprintf(state->logDst, "rs: %d ", rs);
    fprintf(state->logDst, "rt: %d ", rt);
    fprintf(state->logDst, "rd: %d ", rd);
    fprintf(state->logDst, "shift: %x ", shift);
    fprintf(state->logDst, "fn: %x ", fn);
    fprintf(state->logDst, "src1: %d ", src1);
    fprintf(state->logDst, "src2: %d ", src2);
    fprintf(state->logDst, "result: %d ", result);
    fprintf(state->logDst, "msg: %s\n", InsCSV::get_msg());
    
    debugPrintWord(state, Build(), "InsRCSV Built Word:");
}
