#include "test_mips_Jcsv.h"

Jcsv::Jcsv(string nameIn, uint8_t opcodeIn, uint32_t argIn,
             uint32_t resultIn, uint16_t exceptionIn, uint8_t JumpIn, string msgIn):

			InsCSV(nameIn, opcodeIn, exceptionIn, msgIn, JumpIn), arg(argIn),
			result(resultIn) {
                if(result==0) calcResult=1;
                else calcResult=0;
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


void Jcsv::printInsObj(FILE *dest, mips_error err){
    fprintf(dest, "Jcsv Object values: ");
    fprintf(dest, "name: %s ", InsCSV::get_name());
    fprintf(dest, "opcode: %x ", InsCSV::opcode);
    fprintf(dest, "arg: %x ", arg);
    fprintf(dest, "result: %x ", result);
    fprintf(dest, "exception: %x ", InsCSV::exception);
    fprintf(dest, "Jump: %d ", Jump);
    fprintf(dest, "msg: %s\n", InsCSV::get_msg());
    
    if(err!=InsCSV::exception) fprintf(dest, "Expected exception: %x, received %x\n", InsCSV::exception, err);
    else{
        if(calcResult!=result) fprintf(dest, "Expected result: %d, received %d\n", result, calcResult);
    }
}
