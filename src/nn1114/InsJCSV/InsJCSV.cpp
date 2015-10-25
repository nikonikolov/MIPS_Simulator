#include "InsJCSV.h"

InsJCSV::InsJCSV(char* nameIn, uint8_t opcodeIn, uint32_t argIn,
             uint32_t resultIn, char* msgIn) :

			InsCSV(nameIn, opcodeIn, msgIn), arg(argIn),
			result(resultIn) {}


uint32_t InsJCSV::Build(){
    return
        (InsCSV::opcode << 26)  |   // opcode
        (arg << 0);          		// jump argument
}


void InsJCSV::SetRegs(mips_cpu_h cpuPtr){
    // do nothing
}
    

int InsJCSV::CheckResult(mips_cpu_h cpuPtr, char** msg){
    uint32_t calcResult;

    // PROBABLY YOU NEED TO READ PC
    //mips_error err = mips_cpu_get_register(cpuPtr, rd, &calcResult);
    //checkRegGet(err);

    // Modify message
    *msg = InsCSV::msg;

    return result == calcResult;
}


void InsJCSV::printInsObj(mips_cpu_h state){
    fprintf(state->logDst, "InsJCSV Object values: ");
    fprintf(state->logDst, "name: %s ", InsCSV::name);
    fprintf(state->logDst, "opcode: %x ", InsCSV::opcode);
    fprintf(state->logDst, "arg: %x ", arg);
    fprintf(state->logDst, "result: %x ", result);
    fprintf(state->logDst, "msg: %s\n", InsCSV::msg);
    
    debugPrintWord(state, Build(), "InsJCSV Built Word:");
}
