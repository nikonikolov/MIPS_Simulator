#include "test_mips_Icsv.h"

// Non-Jump Constructor - 9 arguments
Icsv::Icsv(string nameIn, uint8_t opcodeIn, uint8_t rsIn, uint8_t rdIn, uint16_t immIn,
             uint32_t src1In, uint32_t resultIn, uint16_t exceptionIn, string msgIn):

			CONINSCSV, rs(rsIn), rd(rdIn), imm(immIn), src1(src1In) {}

// Jump Constructor - 13 arguments
Icsv::Icsv(string nameIn, uint8_t opcodeIn, uint8_t rsIn, uint8_t rdIn, uint16_t immIn,
             uint32_t src1In, uint32_t src2In, int JumpIn, bool JumpTakeIn, bool LinkIn,
             uint32_t resultIn, uint16_t exceptionIn,  string msgIn):

            CONINSCSVJUMP, rs(rsIn), rd(rdIn), imm(immIn), 
            src1(src1In), src2(src2In), JumpTake(JumpTakeIn) {}


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
    if(Jump){
        mips_error err = mips_cpu_set_register(cpuPtr, rd, src2);
        checkRegSet(err);
    }
}
    
int Icsv::JumpNotTaken(mips_cpu_h cpuPtr, mips_error excep_got){
    // The instruction itself
    if(JumpTmp==3){
        mips_cpu_get_pc(cpuPtr, &calcResult);
        JumpTmp--;
        return 1;
    }
        
    // Instruction after the jump
    else if(JumpTmp==2){
        uint32_t PCtmp=calcResult;
        mips_cpu_get_pc(cpuPtr, &calcResult);
        JumpTmp--;

        if (excep_got == InsCSV::exception){
            if (excep_got != mips_Success) return 1;
            else return calcResult==(PCtmp+4);
        }
        else return 0;
    }

    // Destination address
    else if(JumpTmp==1){
        uint32_t PCtmp=calcResult;
        mips_cpu_get_pc(cpuPtr, &calcResult);
        JumpTmp--;

        return calcResult==(PCtmp+4);
    }
        
    else return 0;
}    


int Icsv::JumpTaken(mips_cpu_h cpuPtr, mips_error excep_got){
    
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


int Icsv::CheckResult(mips_cpu_h cpuPtr, mips_error excep_got, char** msg){

    // Modify message
    *msg = InsCSV::get_msg();

    // If Ins is a Jump
    if(JumpTmp){
        if(JumpTake) return JumpTaken(cpuPtr, excep_got);
        return JumpNotTaken(cpuPtr, excep_got);
    }

    // If not a Jump
    mips_error err = mips_cpu_get_register(cpuPtr, rd, &calcResult);
    checkRegGet(err);

    // NOTE : C STORES NEGATIVE NUMBERS INSIDE UINT TYPE WITH THEIR TWO'S COMLEMENT EQUIVALENT, SO NO ADDITIONAL CHECKS ARE REQUIRED
    if (excep_got == InsCSV::exception){
        if (excep_got != mips_Success) return 1;
        else return result == calcResult;
    }
    
    else return 0;
}

void Icsv::printInsObj(FILE *dest, mips_error err){
    fprintf(dest, "Icsv Object values: ");
    fprintf(dest, "name: %s ", InsCSV::get_name());
    fprintf(dest, "opcode: %x ", InsCSV::opcode);
    fprintf(dest, "rs: %d ", rs);
    fprintf(dest, "rd: %d ", rd);
    fprintf(dest, "imm: %d ", imm);
    fprintf(dest, "src1: %d ", src1);
    if(Jump) fprintf(dest, "src2: %d ", src2);
    fprintf(dest, "result: %d ", result);
    fprintf(dest, "exception: %x ", InsCSV::exception);
    if(Jump) fprintf(dest, "Jump: %d ", Jump);
    fprintf(dest, "msg: %s\n", InsCSV::get_msg());
    
    if(err!=InsCSV::exception) fprintf(dest, "Expected exception: %x, received %x\n", InsCSV::exception, err);
    else{
        if(calcResult!=result) fprintf(dest, "Expected result: %d, received %d\n", result, calcResult);
    }
}

