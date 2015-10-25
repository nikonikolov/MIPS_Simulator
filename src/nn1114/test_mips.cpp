#include "mips.h"
#include "test_mips_private.h"
#include "test_mips_exitcheck.h"

#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main()
{
    cout<<"Enter debug level"<<endl;
    cout<<"DISABLED: 0"<<endl;
    cout<<"LOW: 1"<<endl;
    cout<<"MIDDLE: 2"<<endl;
    cout<<"HIGH: 3"<<endl;
    unsigned debuglvl;
    cin>>debuglvl;
    // debuglvl = 0;
   
    /* *********************** DECLARE VARS *********************** */ 
    mips_mem_h mem;
    mips_cpu_h cpu;
    mips_error err;

    vector<InsCSV*> InsObjPtrs;

    int testId;

    /* *********************** CREATE CPU AND RAM *********************** */ 
    mem=mips_mem_create_ram(4096, 4);
    
    cpu=mips_cpu_create(mem);
    
    err=mips_cpu_set_debug_level(cpu, debuglvl, stderr);
    checkDebug(err);
    

    /* *********************** READ/INITIALIZE VECTOR *********************** */ 

    readFile("Instructions.csv", InsObjPtrs);


    /*if(debuglvl>0){
        for(int i=0; i<InsObjPtrs.size();i++){
            InsObjPtrs[i]->printInsObj(cpu);
        }
    }*/


    /* *********************** START TEST SUITE *********************** */ 
    mips_test_begin_suite();


    /* *********************** RUN THE WHOLE PROGRAM LOADED IN MEMORY *********************** */ 
    
    
    /*
    // LOAD INSTRUCTIONS IN RAM
    err = loadMem(mem, InsObjPtrs);
    uint32_t pc=0;
    for(int j=0, i=0; j<InsObjPtrs.size() && pc<InsObjPtrs.size()*4; j++){
        i = pc/4;
        // 1 - set testId
        testId = mips_test_begin_test(InsObjPtrs[i]->get_name());    
    
        // 2 - put register values in cpu
        InsObjPtrs[i]->SetRegs(cpu);
    
        // 3 - step CPU
        err=mips_cpu_step(cpu);
        checkStep(err);
        
        // 4 -Check the result
        char* msg = NULL;
    
        int passed = InsObjPtrs[i]->CheckResult(cpu, err, &msg);
    
        // Print Ins Obj if instruction failed
        if(!passed){
            cout<<"Following instruction failed"<<endl;
            InsObjPtrs[i]->printInsObj(cpu);
        } 

        mips_test_end_test(testId, passed, msg);

        // 5 - Set pc manually if step returned an exception or failed
        if(err){
            err = mips_cpu_set_pc(cpu, pc+4);
            if(err) cout<<" Could not set pc after step failed"<<endl;
        }

        // 6 - update values for next loop run
        err = mips_cpu_get_pc(cpu, &pc);
        checkPCGet(err);
    }
    */

    /* *********************** END RUN THE WHOLE PROGRAM LOADED IN MEMORY *********************** */ 
    

    /* *********************** RUN ONE INSTRUCTION AT A TIME *********************** */ 

    uint32_t pc=0;

    for(int j=0, i=0; j<InsObjPtrs.size() && pc<InsObjPtrs.size()*4; j++){

        i = pc/4;
    
        // 1 - load ONE instruction in RAM, the one that pc currently points to
        err = loadIns(mem, pc, InsObjPtrs[i]);

        testId = mips_test_begin_test(InsObjPtrs[i]->get_name());    
    
        // 2 - put register values in cpu
        InsObjPtrs[i]->SetRegs(cpu);
    
        // 3 - step CPU
        err=mips_cpu_step(cpu);
        
        // 4 -Check the result
        char* msg = NULL;
    
        int passed = InsObjPtrs[i]->CheckResult(cpu, err, &msg);
    
        // Print Ins Obj if instruction failed
        if(!passed){
            cout<<"Following instruction failed"<<endl;
            InsObjPtrs[i]->printInsObj(cpu);
        } 
        
        mips_test_end_test(testId, passed, msg);

        // 5 - Set pc manually if step returned an exception or failed
        if(err){
            err = mips_cpu_set_pc(cpu, pc+4);
            if(err) cout<<" Could not set pc after step failed"<<endl;
        }

        // 6 - update values for next loop run
        err = mips_cpu_get_pc(cpu, &pc);
        checkPCGet(err);
    }

    /* *********************** END RUN ONE INSTRUCTION AT A TIME *********************** */ 


    /* *********************** SECOND TEST STARTS *********************** */ 

    /*testId = mips_test_begin_test("addu");    
    
    // 1 - Setup an instruction in ram
    // addu r3, r4, r5
    InsWord = BuildR(4, 5, 0, 0, 0x21);
    
    InsWord = change_endian(InsWord);
    
    err = mips_mem_write(
        mem,                             //!< Handle to target memory
        0,                               //!< Byte address to start transaction at
        4,                               //!< Number of bytes to transfer
        (uint8_t*)&(InsWord)             //!< Receives the target bytes
    );
    checkMemWrite(err);
    
    // 2 - put register values in cpu
    err = emips_cpu_set_register(cpu, 4, 40);
    checkRegSet(err);
    err = mips_cpu_set_register(cpu, 5, 50);
    checkRegSet(err);
    
    // 3 - step CPU
    err=mips_cpu_step(cpu);
    checkStep(err);
    
    // 4 -Check the result
    err = mips_cpu_get_register(cpu, 3, &result);
    checkRegGet(err);
    
   
    passed = got == 0;
    
    mips_test_end_test(testId, passed, "r0 <> 0");*/


    
    /* *********************** END TEST SUITE *********************** */ 
    
    mips_test_end_suite();
    
    return 0;
}