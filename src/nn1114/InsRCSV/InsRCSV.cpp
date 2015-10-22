#include "InsRCSV.h"

InsRCSV::InsRCSV(char* nameIn, uint8_t opcodeIn /*=0*/, uint8_t rsIn, uint8_t rtIn, uint8_t rdIn, uint8_t shiftIn =0, uint8_t fnIn,
			 int src1In, int src2In, int resultIn)	:
			
			InsCSV(nameIn, opcodeIn), rs(rsIN), rt(rtIN), rd(rdIN), shift(shiftIN), fn(fnIN),
			src1(src1In), src2(src2In), result(resultIn) {}


uint32_t InsRCSV::Build(){
    return
        (INSCSV::opcode << 26)  |   // opcode = 0
        (rs << 21)      |   		// reg 1
        (rt << 16)      |   		// reg 2
        (rd << 11)      |   		// dest reg
        (shift << 6)    |   		// shift
        (fn << 0);          		// fn code
}


void InsRCSV::SetRegs(mips_cpu_h cpuPtr){

	// NOTE : YOU NEED TO PERFORM CHECKS ABOUT THE SIGN OF PARAMETERS AND USE TWOS COMPLEMENT IF NECESSARY
    // BE CAREFUL WIHT UNSIGNED TYPES
	mips_error err = mips_cpu_set_register(cpuPtr, rs, src1);
    checkRegSet(err);
    err = mips_cpu_set_register(cpuPtr, rt, src2);
    checkRegSet(err);
}
    

int InsRCSV::CheckResult(mips_cpu_h cpuPtr, char** msg /*=NULL*/){
    uint32_t calcResult;

    mips_error err = mips_cpu_get_register(cpu, rd, &calcResult);
    checkRegGet(err);

    // MODIFY MESSAGE IF NECESSARY. NEED ANOTHER MEMBER IN ABSTRACT CLASS

    // NOTE : YOU NEED TO PERFORM CHECKS ABOUT THE SIGN OF PARAMETERS AND USE TWOS COMPLEMENT IF NECESSARY
    // BE CAREFUL WIHT UNSIGNED TYPES
    return result == calcResult
}
