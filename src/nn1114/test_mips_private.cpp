#include "test_mips_private.h"

uint32_t BuildR(uint32_t opcode /*=0*/, uint32_t rs, uint32_t rt, uint32_t rd, uint32_t shift, uint32_t fn){
    return
        (opcode << 26)  |   // opcode = 0
        (rs << 21)      |   // reg 1
        (rt << 16)      |   // reg 2
        (rd << 11)      |   // dest reg
        (shift << 6)    |   // shift
        (fn << 0);          // fn code
}


uint32_t BuildI(uint32_t opcode, uint32_t rs, uint32_t rd, uint32_t imm){
    return
        (opcode << 26)  |   // opcode
        (rs << 21)      |   // source reg
        (rd << 16)      |   // dest reg
        (imm << 0);         // immediate const
}

uint32_t BuildJ(uint32_t opcode, uint32_t arg){
    return
        (opcode << 26)  |   // opcode
        (fn << 0);          // argument
}

void checkWrap(mips_error err, char* msg){
    if(err!=mips_Success){
        fprintf(stderr,"%s\n", msg);
        fprintf(stderr, "Error code: %u\n" err);
        exit(1);
    }
}

void checkStep(mips_error err){
    char* msg = "mips_cpu_step : failed."
    checkWrap(err, msg);
}

void checkDebug(mips_error err){
    char* msg = "mips_cpu_set_debug_level : failed."
    checkWrap(err, msg);
}

void checkMemWrite(mips_error err){
    char* msg = "mips_mem_write : failed."
    checkWrap(err, msg);
}

void checkMemRead(mips_error err){
    char* msg = "mips_mem_read : failed."
    checkWrap(err, msg);
}

void checkRegSet(mips_error err){
    char* msg = "mips_cpu_set_reg : failed."
    checkWrap(err, msg);
}

void checkRegGet(mips_error err){
    char* msg = "mips_cpu_get_reg : failed."
    checkWrap(err, msg);
}