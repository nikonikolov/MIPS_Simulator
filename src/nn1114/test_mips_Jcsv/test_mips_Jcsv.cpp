#include "test_mips_Jcsv.h"

Jcsv::Jcsv(string nameIn, uint8_t opcodeIn, uint32_t argIn,
             uint32_t resultIn, uint16_t exceptionIn, string msgIn) :

			InsCSV(nameIn, opcodeIn, exceptionIn, msgIn), arg(argIn),
			result(resultIn) {
                if(result==0) calcResult=1;
            }


uint32_t Jcsv::Build(){
    return
        (InsCSV::opcode << 26)  |   // opcode
        (arg << 0);          		// jump argument
}


void Jcsv::SetRegs(mips_cpu_h cpuPtr){
    // do nothing
}
    

int Jcsv::CheckResult(mips_cpu_h cpuPtr, mips_error excep_got, char** msg){

    // PROBABLY YOU NEED TO READ PC
    //mips_error err = mips_cpu_get_register(cpuPtr, rd, &calcResult);
    //checkRegGet(err);

    // Modify message
    *msg = InsCSV::get_msg();

    if (excep_got == InsCSV::exception){
        if (excep_got != mips_Success) return 1;
        else return result == calcResult;
    }
    
    else return 0;
}


void Jcsv::printInsObj(mips_cpu_h state, mips_error err){
    fprintf(state->logDst, "Jcsv Object values: ");
    fprintf(state->logDst, "name: %s ", InsCSV::get_name());
    fprintf(state->logDst, "opcode: %x ", InsCSV::opcode);
    fprintf(state->logDst, "arg: %x ", arg);
    fprintf(state->logDst, "result: %x ", result);
    fprintf(state->logDst, "exception: %x ", InsCSV::exception);
    fprintf(state->logDst, "msg: %s\n", InsCSV::get_msg());
    
    if(state->logLevel>=1) debugPrintWord(state, Build(), "Jcsv Built Word:");
    
    if(err!=InsCSV::exception) fprintf(state->logDst, "Expected exception: %x, received %x\n", InsCSV::exception, err);
    else{
        if(calcResult!=result) fprintf(state->logDst, "Expected result: %d, received %d\n", result, calcResult);
    }
}