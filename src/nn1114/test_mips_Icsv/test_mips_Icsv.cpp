#include "test_mips_Icsv.h"

Icsv::Icsv(string nameIn, uint8_t opcodeIn, uint8_t rsIn, uint8_t rdIn, uint16_t immIn,
             uint32_t src1In, uint32_t resultIn, uint16_t exceptionIn, string msgIn):

			InsCSV(nameIn, opcodeIn, exceptionIn, msgIn), rs(rsIn), rd(rdIn), imm(immIn),
			src1(src1In), result(resultIn) {
                if(result==0) calcResult=1;
            }


uint32_t Icsv::Build(){
    return
        (InsCSV::opcode << 26)  |   // opcode
        (rs << 21)      |   		// reg 1
        (rd << 16)      |   		// dest reg
        (imm << 0);          		// immediate const
}


void Icsv::SetRegs(mips_cpu_h cpuPtr){

    // NOTE : C STORES NEGATIVE NUMBERS INSIDE UINT TYPE WITH THEIR TWO'S COMLEMENT EQUIVALENT, SO NO ADDITIONAL CHECKS ARE REQUIRED
	mips_error err = mips_cpu_set_register(cpuPtr, rs, src1);
    checkRegSet(err);
}
    

// WHAT ABOUT JUMPS ???
int Icsv::CheckResult(mips_cpu_h cpuPtr, mips_error excep_got, char** msg){

    mips_error err = mips_cpu_get_register(cpuPtr, rd, &calcResult);
    checkRegGet(err);

    // Modify message
    *msg = InsCSV::get_msg();

    // NOTE : C STORES NEGATIVE NUMBERS INSIDE UINT TYPE WITH THEIR TWO'S COMLEMENT EQUIVALENT, SO NO ADDITIONAL CHECKS ARE REQUIRED
    if (excep_got == InsCSV::exception){
        if (excep_got != mips_Success) return 1;
        else return result == calcResult;
    }
    
    else return 0;
}

void Icsv::printInsObj(mips_cpu_h state, mips_error err){
    fprintf(state->logDst, "Icsv Object values: ");
    fprintf(state->logDst, "name: %s ", InsCSV::get_name());
    fprintf(state->logDst, "opcode: %x ", InsCSV::opcode);
    fprintf(state->logDst, "rs: %d ", rs);
    fprintf(state->logDst, "rd: %d ", rd);
    fprintf(state->logDst, "imm: %d ", imm);
    fprintf(state->logDst, "src1: %d ", src1);
    fprintf(state->logDst, "result: %d ", result);
    fprintf(state->logDst, "exception: %x ", InsCSV::exception);
    fprintf(state->logDst, "msg: %s\n", InsCSV::get_msg());
    
    if(state->logLevel>=1) debugPrintWord(state, Build(), "Icsv Built Word:");
    if(err!=InsCSV::exception) fprintf(state->logDst, "Expected exception: %x, received %x\n", InsCSV::exception, err);
    else{
        if(calcResult!=result) fprintf(state->logDst, "Expected result: %d, received %d\n", result, calcResult);
    }
}

