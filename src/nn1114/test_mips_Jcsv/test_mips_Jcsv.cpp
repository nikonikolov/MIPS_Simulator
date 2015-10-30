#include "test_mips_Jcsv.h"

// Jump Constructor - 8 arguments
Jcsv::Jcsv(string nameIn, uint8_t opcodeIn, uint32_t argIn,
             int JumpIn, bool LinkIn, uint32_t resultIn, uint16_t exceptionIn, string msgIn) :

			CONINSCSVJUMP, arg(argIn) {}


uint32_t Jcsv::Build(){
    return
        (InsCSV::opcode << 26)  |   // opcode
        (arg << 0);          		// jump argument
}


void Jcsv::SetRegs(mips_cpu_h cpuPtr){
    // do nothing
}
    

int Jcsv::CheckResult(mips_cpu_h cpuPtr, mips_mem_h mem, mips_error excep_got, char** msg){

    // Modify message
    *msg = InsCSV::get_msg();

    // PC points to the instruction itself
    if(JumpTmp==3){
        mips_cpu_get_pc(cpuPtr, &calcResult);
        JumpTmp--;
        return 1;
    }
        
    // PC points to instruction after the jump
    else if(JumpTmp==2){
        uint32_t PCtmp=calcResult;
        mips_cpu_get_pc(cpuPtr, &calcResult);
        JumpTmp--;

        if(Link){
            uint32_t returnAddr;
            mips_cpu_get_register(cpuPtr, 31, &returnAddr);
            if(returnAddr != (calcResult+4) ) return 0;
        }

        if (excep_got == InsCSV::exception){
            if (excep_got != mips_Success) return 1;
            else return calcResult==(PCtmp+4);
        }
        else return 0;
    }

    // PC points to destination address
    else if(JumpTmp==1){
        mips_cpu_get_pc(cpuPtr, &calcResult);
        JumpTmp--;

        return result==calcResult;
    }
        
    else return 0;
}


void Jcsv::printInsObj(FILE *dest, mips_error err){
    fprintf(dest, "Jcsv Object values: ");
    fprintf(dest, "name: %s ", InsCSV::get_name());
    fprintf(dest, "opcode: %x ", InsCSV::opcode);
    fprintf(dest, "arg: %x ", arg);
    fprintf(dest, "Jump: %d ", Jump);
    fprintf(dest, "Link: %d ", (int)Link);
    fprintf(dest, "result: %x ", result);
    fprintf(dest, "exception: %x ", InsCSV::exception);
    fprintf(dest, "msg: %s\n", InsCSV::get_msg());
    
    if(err!=InsCSV::exception) fprintf(dest, "Expected exception: %x, received %x\n", InsCSV::exception, err);
    else{
        if(calcResult!=result) fprintf(dest, "Expected result: %d, received %d\n", result, calcResult);
    }
}
