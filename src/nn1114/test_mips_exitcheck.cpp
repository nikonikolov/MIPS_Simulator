#include "test_mips_exitcheck.h"
 

void checkWrap(mips_error err, char* msg){
    if(err!=mips_Success){
        fprintf(stderr,"%s\n", msg);
        fprintf(stderr, "Error code: %x\n", err);
        exit(1);
    }
}

void checkStep(mips_error err){
    char* msg = "mips_cpu_step : failed.";
    checkWrap(err, msg);
}

void checkDebug(mips_error err){
    char* msg = "mips_cpu_set_debug_level : failed.";
    checkWrap(err, msg);
}

void checkMemWrite(mips_error err){
    char* msg = "mips_mem_write : failed.";
    checkWrap(err, msg);
}

void checkMemRead(mips_error err){
    char* msg = "mips_mem_read : failed.";
    checkWrap(err, msg);
}

void checkRegSet(mips_error err){
    char* msg = "mips_cpu_set_reg : failed.";
    checkWrap(err, msg);
}

void checkRegGet(mips_error err){
    char* msg = "mips_cpu_get_reg : failed.";
    checkWrap(err, msg);
}

void checkPCGet(mips_error err){
    char* msg = "mips_cpu_get_pc : failed.";
    checkWrap(err, msg);
}

