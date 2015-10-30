#include "mips.h"
#include "test_mips_private.h"
#include "test_mips_exitcheck.h"

#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main()
{
    /*cout<<"Enter debug level"<<endl;
    cout<<"DISABLED: 0"<<endl;
    cout<<"LOW: 1"<<endl;
    cout<<"MIDDLE: 2"<<endl;
    cout<<"HIGH: 3"<<endl;*/
    unsigned debuglvl=0;
    //cin>>debuglvl;
   
    /* *********************** DECLARE VARS *********************** */ 
    mips_mem_h mem;
    mips_cpu_h cpu;
    mips_error err;

    vector<InsCSV*> InsObjPtrs;

    int testId, idx, jumpPassed=1, passed;

    char* msg = NULL;

    int jump=0;

    FILE *dest = stderr;

    /* *********************** CREATE CPU AND RAM *********************** */ 
    mem=mips_mem_create_ram(4096, 4);
    
    cpu=mips_cpu_create(mem);
    
    err=mips_cpu_set_debug_level(cpu, debuglvl, dest);
    checkDebug(err);
    

    /* *********************** READ/INITIALIZE VECTOR *********************** */ 

    //readFile("Jumps.csv", InsObjPtrs);
    //readFile("Instructions.csv", InsObjPtrs);
    readFile("JumpsNew.csv", InsObjPtrs);
    readFile("InstructionsRNew.csv", InsObjPtrs);
    readFile("InstructionsINew.csv", InsObjPtrs);

    /*if(debuglvl>0){
        for(int i=0; i<InsObjPtrs.size();i++){
            InsObjPtrs[i]->printInsObj(dest);
        }
    }*/


    /* *********************** START TEST SUITE *********************** */ 
    mips_test_begin_suite();


    /* *********************** RUN THE WHOLE PROGRAM LOADED IN MEMORY *********************** */ 
    
    
    
    // LOAD INSTRUCTIONS IN RAM
    err = loadMem(mem, InsObjPtrs);
    
    uint32_t pc=0;
    
    for(int j=0, i=0; j<InsObjPtrs.size() && pc<InsObjPtrs.size()*4; j++){
        
        // 1 - Syncronize program counter and InsObjPrts Element
        i = pc/4;
        
        // 2 - If Jump is not being tested currenlty, check if current instruction is a jump
        if(jump==0){
            
            // 1 - Start test for current instruction
            testId = mips_test_begin_test(InsObjPtrs[i]->get_name());    
        
            jump = InsObjPtrs[i]->get_Jump();
            
            // 1 - If current instruction is a jump, check current PC and update args
            if(jump!=0){
                idx = i;
                passed = InsObjPtrs[i]->CheckResult(cpu, mem, mips_Success, &msg);
                jumpPassed = passed;            
            }
        }

        // 3 - put register values in cpu
        InsObjPtrs[i]->SetRegs(cpu);
    
        // 4 - step CPU
        err = mips_cpu_step(cpu);

        // 5 - Set pc manually if step returned an exception or failed
        if(err){
            // If Jump is being tested, check whether expected error is received
            // Replace instruction with NOOP and step pc
            if(jump>0){
                // 1 -Check whether the expected exception is returned
                passed = InsObjPtrs[i]->CheckResult(cpu, mem, err, &msg);

                // 2 - Print Ins Obj if instruction failed
                if(!passed){
                    cout<<"Following instruction failed"<<endl;
                    InsObjPtrs[i]->printInsObj(dest, err);
                } 
                
                uint32_t Word = 0;
                // 3 - Replace current instruction with a NOOP
                mips_mem_write(
                    mem,                             //!< Handle to target memory
                    pc,                              //!< Byte address to start transaction at
                    4,                               //!< Number of bytes to transfer
                    (uint8_t*)&(Word)                   //!< Receives the target bytes
                );

                // Step CPU
                mips_cpu_step(cpu);       // SHOULD YOU CHECK ERROR CODE
            }
            
            // If jump is not being tested, set PC manually
            else{
                mips_error errtmp = mips_cpu_set_pc(cpu, pc+4);    
                if(errtmp) cout<<" Could not set pc after step failed"<<endl;
            } 
        }

        
        // 6 - If Jump is currenlty being tested, check result
        if(jump>0){ 
            
            // Pass relevant error error code to CheckResult
            mips_error errtmp = mips_Success;
            if(jump==2) errtmp = err;

            // 1 - Check the result
            int tmpJumpPassed = InsObjPtrs[idx]->CheckResult(cpu, mem, errtmp, &msg);
            jumpPassed = tmpJumpPassed & jumpPassed;

            // 2 - Check whether testing is over
            jump = InsObjPtrs[idx]->get_Jump();
            
            if(jump==0){

                // Print Ins Obj if instruction failed
                if(!jumpPassed){
                    cout<<"Following instruction failed"<<endl;
                    // MAKE SURE YOU SEND PROPER ERROR CODE
                    InsObjPtrs[idx]->printInsObj(dest, mips_Success);
                }

                // 3 - Finish Jump Instruction Test
                mips_test_end_test(testId, jumpPassed, msg);
               
                // 4 - Reset jumpPassed value for next Jump Instruction
                jumpPassed = 1;
                
    // ********* Jump was being tested by now. You CANNOT start test for the current instruction because
    // ********* it was already executed and the test will NOT see the required changes
            }
        }
        

        // 6 - If Jump is not being currently tested
        else{
            // 7 -Check the result of current instruction
            int passed = InsObjPtrs[i]->CheckResult(cpu, mem, err, &msg);

            if(jump<0) passed = jumpPassed & passed;

            // Print Ins Obj if instruction failed
            if(!passed){
                cout<<"Following instruction failed"<<endl;
                InsObjPtrs[i]->printInsObj(dest, err);
            } 
                
            // 8 - Finish Current Instruction Test
            mips_test_end_test(testId, passed, msg);

            // If instruction was a store, update jump for next loop run
            if(jump<0) jump=0;
        }

        // FINAL - update values for next loop run
        err = mips_cpu_get_pc(cpu, &pc);
        checkPCGet(err);
    }
    

    /* *********************** END RUN THE WHOLE PROGRAM LOADED IN MEMORY *********************** */ 
    

    /* *********************** RUN ONE INSTRUCTION AT A TIME *********************** */ 

    /*uint32_t pc=0;

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
    
        int passed = InsObjPtrs[i]->CheckResult(cpu, mem, err, &msg);
    
        // Print Ins Obj if instruction failed
        if(!passed){
            cout<<"Following instruction failed"<<endl;
            InsObjPtrs[i]->printInsObj(dest, err);
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
    }*/

    /* *********************** END RUN ONE INSTRUCTION AT A TIME *********************** */ 
    
    /* *********************** END TEST SUITE *********************** */ 
    
    mips_test_end_suite();
    

    return 0;
}

/*    for(int j=0, i=0; j<InsObjPtrs.size() && pc<InsObjPtrs.size()*4; j++){
        
        // 1 - Syncronize program counter and InsObjPrts Element
        i = pc/4;
        
        // 2 - If Jump is not being tested currenlty, check if current instruction is a jump
        if(jump==0){
            jump = InsObjPtrs[i]->get_Jump();
            
            // 1 - If current instruction is a jump, check current PC and update args
            if(jump){
                idx = i;
                passed = InsObjPtrs[i]->CheckResult(cpu, mem, mips_Success, &msg);
                jumpPassed = passed;            
            }
        }

        // 3 - put register values in cpu
        InsObjPtrs[i]->SetRegs(cpu);
    
        // 4 - step CPU
        err = mips_cpu_step(cpu);

        // 5 - Set pc manually if step returned an exception or failed
        if(err){
            mips_error errtmp = mips_cpu_set_pc(cpu, pc+4);
            if(errtmp) cout<<" Could not set pc after step failed"<<endl;
        }

        
        // 6 - If Jump is currenlty being tested, check result
        if(jump){ 
            // PASS THE PROPER ERROR CODE TO THE CHECK RESULT, MSG IS FINE, GETS UPDATED
            
            // 1 - Check the result
            int tmpJumpPassed = InsObjPtrs[idx]->CheckResult(cpu, mem, mips_Success, &msg);
            jumpPassed = tmpJumpPassed & jumpPassed;

            // 2 - Check whether testing is over
            jump = InsObjPtrs[idx]->get_Jump();
            
            if(jump==0){

                // Print Ins Obj if instruction failed
                if(!jumpPassed){
                    cout<<"Following instruction failed"<<endl;
                    // MAKE SURE YOU SEND PROPER ERROR CODE
                    InsObjPtrs[idx]->printInsObj(dest, mips_Success);
                }

                // 3 - Record Current Instruction Test
                testId = mips_test_begin_test(InsObjPtrs[idx]->get_name());    
                mips_test_end_test(testId, jumpPassed, msg);
               
                // 4 - Reset jumpPassed value for next Jump Instruction
                jumpPassed = 1;
            
                
    // ********* Jump has ended, find out whether current ins is a jump and if not, finish the current test
                
                // 5 -Check the result
                int passed = InsObjPtrs[i]->CheckResult(cpu, mem, err, &msg);

                // get_Jump returns 0 after it's last test has finished, hence if only 1 test is required
                // it is not a jump instruction
                // 6 - Check if current instruction is a Jump
                jump = InsObjPtrs[i]->get_Jump();
            
                // 7 - If current instruction is a Jump, update values
                if(jump){
                    idx = i;
                    jumpPassed = passed;
                }

                // 8 - If not a Jump, Finish current test
                else{
                    // Print Ins Obj if instruction failed
                    if(!passed){
                        cout<<"Following instruction failed"<<endl;
                    InsObjPtrs[i]->printInsObj(dest, err);
                    } 

                    // 9 - Record Current Instruction Test
                    testId = mips_test_begin_test(InsObjPtrs[i]->get_name());    
                    mips_test_end_test(testId, passed, msg);
                }
            }
        }
        

        // 6 - If Jump is not being currently tested
        else{
            // 7 -Check the result of current instruction
            int passed = InsObjPtrs[i]->CheckResult(cpu, mem, err, &msg);

            // Print Ins Obj if instruction failed
            if(!passed){
                cout<<"Following instruction failed"<<endl;
                InsObjPtrs[i]->printInsObj(dest, err);
            } 
                
            // 8 - Record Current Instruction Test
            testId = mips_test_begin_test(InsObjPtrs[i]->get_name());    
            mips_test_end_test(testId, passed, msg);
        }

        // FINAL - update values for next loop run
        err = mips_cpu_get_pc(cpu, &pc);
        checkPCGet(err);
    }
    */