#include "test_mips_Jcsv.h"

Jcsv::Jcsv(string nameIn, uint8_t opcodeIn, uint32_t argIn,
             uint32_t resultIn, string msgIn) :

			InsCSV(nameIn, opcodeIn, msgIn), arg(argIn),
			result(resultIn) {}


uint32_t Jcsv::Build(){
    return
        (InsCSV::opcode << 26)  |   // opcode
        (arg << 0);          		// jump argument
}


void Jcsv::SetRegs(mips_cpu_h cpuPtr){
    // do nothing
}
    

int Jcsv::CheckResult(mips_cpu_h cpuPtr, char** msg){
    uint32_t calcResult;

    // PROBABLY YOU NEED TO READ PC
    //mips_error err = mips_cpu_get_register(cpuPtr, rd, &calcResult);
    //checkRegGet(err);

    // Modify message
    *msg = InsCSV::get_msg();

    return result == calcResult;
}


void Jcsv::printInsObj(mips_cpu_h state){
    fprintf(state->logDst, "Jcsv Object values: ");
    fprintf(state->logDst, "name: %s ", InsCSV::get_name());
    fprintf(state->logDst, "opcode: %x ", InsCSV::opcode);
    fprintf(state->logDst, "arg: %x ", arg);
    fprintf(state->logDst, "result: %x ", result);
    fprintf(state->logDst, "msg: %s\n", InsCSV::get_msg());
    
    debugPrintWord(state, Build(), "Jcsv Built Word:");
}
