#include "test_mips_Icsv.h"

#include <iostream>
using namespace std;

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
        if(opcode!=0x01 && opcode!=0x07 && opcode!=0x06 && opcode!=0x23 && opcode!=0x21 && opcode!=0x0F && opcode!=0x25 && opcode!=0x24 && opcode!=0x20){
            mips_error err = mips_cpu_set_register(cpuPtr, rd, src2);
        }
        checkRegSet(err);
    }
}
    

int Icsv::CheckResult(mips_cpu_h cpuPtr, mips_mem_h mem, mips_error excep_got, char** msg){

    // Modify message
    *msg = InsCSV::get_msg();

    // If Ins is a Jump
    if(Jump>0){
        if(JumpTake) return JumpTaken(cpuPtr, excep_got);
        return JumpNotTaken(cpuPtr, excep_got);
    }

    // If Ins is a Load/Store
    if(Jump<0){
        if(Jump==-1 || Jump==-2 || Jump==-3) return CALLSLCHECK(StoreCheck);//(cpuPtr, mem, excep_got);
        else return LoadCheck(cpuPtr, mem, excep_got);
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
    
    if(Jump){
        fprintf(dest, "src2: %d\n", src2);  
        fprintf(dest, "Jump: %d ", Jump);
        fprintf(dest, "JumpTake: %d ", (int)JumpTake);
        fprintf(dest, "Link: %d ", (int)Link);
    } 

    fprintf(dest, "result: %d ", result);
    fprintf(dest, "exception: %x ", InsCSV::exception);
    fprintf(dest, "msg: %s\n", InsCSV::get_msg());
    
    if(err!=InsCSV::exception) fprintf(dest, "Expected exception: %x, received %x\n", InsCSV::exception, err);
    else{
        if(calcResult!=result) fprintf(dest, "Expected result: %d, received %d\n", result, calcResult);
    }
}


int Icsv::JumpNotTaken(mips_cpu_h cpuPtr, mips_error excep_got){
    // The instruction itself
    if(JumpTmp==3){
        mips_cpu_get_pc(cpuPtr, &calcResult);
        JumpTmp--;
        if(!Link) mips_cpu_get_register(cpuPtr, 31, &reg31);
        return 1;
    }
        
    // Instruction after the jump
    else if(JumpTmp==2){
        uint32_t PCtmp=calcResult;
        mips_cpu_get_pc(cpuPtr, &calcResult);
        JumpTmp--;

        if(Link){
            uint32_t returnAddr;
            mips_cpu_get_register(cpuPtr, 31, &returnAddr);
            if(returnAddr != (calcResult+4) ) return 0;
        }
        else{
            uint32_t reg31new;
            mips_cpu_get_register(cpuPtr, 31, &reg31new);
            if(reg31new!=reg31) return 0;
        }

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
        if(!Link) mips_cpu_get_register(cpuPtr, 31, &reg31);
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
        else{
            uint32_t reg31new;
            mips_cpu_get_register(cpuPtr, 31, &reg31new);
            if(reg31new!=reg31) return 0;
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

int Icsv::StoreCheck(mips_cpu_h cpuPtr, mips_mem_h mem, mips_error excep_got){
    
    // Check Mem before execution
    if(JumpTmp==Jump){
        mips_mem_read(
            mem,                                //!< Handle to target memory
            (result-result%4),                  //!< Byte address to start transaction at
            4,                                  //!< Number of bytes to transfer
            (uint8_t*)&(calcResult)             //!< Receives the target bytes
        );

        JumpTmp++;
        return 1;
    } 


    else if(JumpTmp==(Jump+1)){
        // Allow for another execution of this instruction
        JumpTmp--;

        // Word
        if(Jump==-1) return CALLSLCHECK(StoreWordCheck);      

        // Half Word
        if(Jump==-2) return CALLSLCHECK(StoreHalfWordCheck);      
        
        // Byte
        if(Jump==-3) return CALLSLCHECK(StoreByteCheck);      

        // JumpTmp == Jump / Jump+1 Jump != -1, -2
        else return 0;
    }

    // JumpTmp != Jump / Jump+1
    else return 0;
}

int Icsv::StoreWordCheck(mips_cpu_h cpuPtr, mips_mem_h mem, mips_error excep_got){
                
    if(excep_got != exception) return 0;

    uint32_t prevCont=calcResult;
    // NOTE: result holds the resulting address and src2 holds its contents
    mips_mem_read(
        mem,                                //!< Handle to target memory
        result,                             //!< Byte address to start transaction at
        4,                                  //!< Number of bytes to transfer
        (uint8_t*)&(calcResult)             //!< Receives the target bytes
    );
    
            
    if(excep_got == mips_Success) {
        // Change endian before you compare
        calcResult = test_nn1114_change_endian(calcResult);

        return src2 == calcResult;
    }

    // If Exception was detected, check if address was modified
    else if(excep_got == mips_ExceptionInvalidAddress) return calcResult == prevCont;
            
    // If any other exception
    else return 0;
}

int Icsv::StoreHalfWordCheck(mips_cpu_h cpuPtr, mips_mem_h mem, mips_error excep_got){

    if(excep_got != exception) return 0;

    uint32_t prevCont=calcResult;
    mips_mem_read(
        mem,                                //!< Handle to target memory
        (result-result%4),                  //!< Byte address to start transaction at
        4,                                  //!< Number of bytes to transfer
        (uint8_t*)&(calcResult)             //!< Receives the target bytes
    );

    uint32_t src2Aligned = 0xFFFF0000 & test_nn1114_change_endian(src2);
    if(excep_got == mips_Success) {
        if(result%4==2){
            return calcResult == (src2Aligned | (prevCont & 0x0000FFFF) ); 
        } 
        else{
            return calcResult == ( ((src2Aligned>>16) & 0x0000FFFF) | (prevCont & 0xFFFF0000) ); 
        } 
    }

    // If Exception was detected, check if address was modified
    else if(excep_got == mips_ExceptionInvalidAddress) return calcResult == prevCont;
            
    // If any other exception
    else return 0;
}


int Icsv::StoreByteCheck(mips_cpu_h cpuPtr, mips_mem_h mem, mips_error excep_got){
            
    if(excep_got != exception) return 0;

    uint32_t prevCont=calcResult;
    mips_mem_read(
        mem,                                //!< Handle to target memory
        (result-result%4),                  //!< Byte address to start transaction at
        4,                                  //!< Number of bytes to transfer
        (uint8_t*)&(calcResult)             //!< Receives the target bytes
    );

    uint32_t src2Aligned = src2 & 0x000000FF;

    if(excep_got == mips_Success) {
        int i = result%4;
        uint32_t tmp=(src2Aligned << i*8) | (prevCont & ( 0xFFFFFFFF ^ ((0x000000FF)<< i*8) ));

        return calcResult == ((src2Aligned << i*8) | (prevCont & ( 0xFFFFFFFF ^ ((0x000000FF)<< i*8) )));
    }

    // If Exception was detected, check if address was modified
    else if(excep_got == mips_ExceptionInvalidAddress) return calcResult == prevCont;
            
    // If any other exception
    else return 0;
}

int Icsv::LoadCheck(mips_cpu_h cpuPtr, mips_mem_h mem, mips_error excep_got){

    // Check Register before execution
    if(JumpTmp==Jump){
        mips_cpu_get_register(cpuPtr, rd, &calcResult);

        JumpTmp++;
        return 1;
    } 


    else if(JumpTmp==(Jump+1)){
        // Allow for another execution of this instruction
        JumpTmp--;

        // Word
        if(Jump==-10) return CALLSLCHECK(LoadWordCheck);      

        // Half Word
        if(Jump==-12) return CALLSLCHECK(LoadHalfWordCheck);      
        
        // Byte
        if(Jump==-13) return CALLSLCHECK(LoadByteCheck);      

        // JumpTmp == Jump / Jump+1 Jump != -1, -2
        else return 0;
    }

    // JumpTmp != Jump / Jump+1
    else return 0;
}

int Icsv::LoadWordCheck(mips_cpu_h cpuPtr, mips_mem_h mem, mips_error excep_got){
                
    if(excep_got != exception) return 0;

    uint32_t Content, prevCont = calcResult;
    // NOTE: result holds the resulting address and src2 holds its contents
    mips_mem_read(
        mem,                                //!< Handle to target memory
        result,                             //!< Byte address to start transaction at
        4,                                  //!< Number of bytes to transfer
        (uint8_t*)&(Content)             //!< Receives the target bytes
    );
    
    mips_cpu_get_register(cpuPtr, rd, &calcResult);
            
    if(excep_got == mips_Success) {
        calcResult = test_nn1114_change_endian(calcResult);
        return Content == calcResult;
    }

    // If Exception was detected, check if address was modified
    else if(excep_got == mips_ExceptionInvalidAddress) return calcResult == prevCont;
            
    // If any other exception
    else return 0;
}


int Icsv::LoadHalfWordCheck(mips_cpu_h cpuPtr, mips_mem_h mem, mips_error excep_got){


    if(excep_got != exception) return 0;

    uint32_t Content, prevCont = calcResult;
    mips_mem_read(
        mem,                                //!< Handle to target memory
        (result-result%4),                  //!< Byte address to start transaction at
        4,                                  //!< Number of bytes to transfer
        (uint8_t*)&(Content)             //!< Receives the target bytes
    );

    mips_cpu_get_register(cpuPtr, rd, &calcResult);

    uint32_t src2Aligned = 0xFFFF0000 & test_nn1114_change_endian(src2);
    if(excep_got == mips_Success) {
        if(result%4==2){
            Content = ( (Content>>8) & 0x0000FF00) | ( (Content>>24) & 0x000000FF);
        } 
        else{
            Content = ( (Content<<8) & 0x0000FF00) | ( (Content>>8) & 0x000000FF);
        } 
        if(opcode == 0x21){
            if(0x00008000 & Content) Content = Content | 0xFFFF0000;
        }

        return calcResult == Content;
    }

    // If Exception was detected, check if address was modified
    else if(excep_got == mips_ExceptionInvalidAddress) return calcResult == prevCont;
            
    // If any other exception
    else return 0;
}

int Icsv::LoadByteCheck(mips_cpu_h cpuPtr, mips_mem_h mem, mips_error excep_got){}
