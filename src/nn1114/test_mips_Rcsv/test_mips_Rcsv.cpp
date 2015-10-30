#include "test_mips_Rcsv.h"

// Non-Jump Constructor - 12 arguments
Rcsv::Rcsv(string nameIn, uint8_t opcodeIn, uint8_t rsIn, uint8_t rtIn, uint8_t rdIn, uint8_t shiftIn, uint8_t fnIn,
			 uint32_t src1In, uint32_t src2In, uint32_t resultIn, uint16_t exceptionIn, string msgIn)	:
			
			CONINSCSV, rs(rsIn), rt(rtIn), rd(rdIn), shift(shiftIn), fn(fnIn),
			src1(src1In), src2(src2In) {}


// Jump Constructor - 14 arguments
Rcsv::Rcsv(string nameIn, uint8_t opcodeIn, uint8_t rsIn, uint8_t rtIn, uint8_t rdIn, uint8_t shiftIn, uint8_t fnIn,
        uint32_t src1In, uint32_t src2In, int JumpIn, bool LinkIn, uint32_t resultIn, uint16_t exceptionIn, string msgIn) :

            CONINSCSVJUMP, rs(rsIn), rt(rtIn), rd(rdIn), shift(shiftIn), fn(fnIn),
            src1(src1In), src2(src2In) {}


uint32_t Rcsv::Build(){
    return
        (InsCSV::opcode << 26)  |   // opcode = 0
        (rs << 21)      |   		// reg 1
        (rt << 16)      |   		// reg 2
        (rd << 11)      |   		// dest reg
        (shift << 6)    |   		// shift
        (fn << 0);          		// fn code
}


void Rcsv::SetRegs(mips_cpu_h cpuPtr){

    // NOTE : C STORES NEGATIVE NUMBERS INSIDE UINT TYPE WITH THEIR TWO'S COMLEMENT EQUIVALENT, SO NO ADDITIONAL CHECKS ARE REQUIRED
	mips_error err = mips_cpu_set_register(cpuPtr, rs, src1);
    checkRegSet(err);
    err = mips_cpu_set_register(cpuPtr, rt, src2);
    checkRegSet(err);
}
    

int Rcsv::CheckResult(mips_cpu_h cpuPtr, mips_error excep_got, char** msg){

    // If not a Jump
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

void Rcsv::printInsObj(FILE *dest, mips_error err){
    fprintf(dest, "Rcsv Object values: ");
    fprintf(dest, "name: %s ", InsCSV::get_name());
    fprintf(dest, "opcode: %x ", InsCSV::opcode);
    fprintf(dest, "rs: %d ", rs);
    fprintf(dest, "rt: %d ", rt);
    fprintf(dest, "rd: %d ", rd);
    fprintf(dest, "shift: %x ", shift);
    fprintf(dest, "fn: %x ", fn);
    fprintf(dest, "src1: %d ", src1);
    fprintf(dest, "src2: %d ", src2);
    fprintf(dest, "result: %d ", result);
    fprintf(dest, "exception: %x ", InsCSV::exception);
    fprintf(dest, "msg: %s\n", InsCSV::get_msg());
    
    if(err!=InsCSV::exception) fprintf(dest, "Expected exception: %x, received %x\n", InsCSV::exception, err);
    else{
        if(calcResult!=result) fprintf(dest, "Expected result: %d, received %d\n", result, calcResult);
    }
}
