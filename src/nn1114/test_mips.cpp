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

    int testId, idx, jumpPassed=1, passed;

    char* msg = NULL;

    uint8_t jump=0;

    FILE *dest = stderr;

    /* *********************** CREATE CPU AND RAM *********************** */ 
    mem=mips_mem_create_ram(4096, 4);
    
    cpu=mips_cpu_create(mem);
    
    err=mips_cpu_set_debug_level(cpu, debuglvl, dest);
    checkDebug(err);
    

    /* *********************** READ/INITIALIZE VECTOR *********************** */ 

    readFile("Jumps.csv", InsObjPtrs);
    readFile("Instructions.csv", InsObjPtrs);

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
            jump = InsObjPtrs[i]->get_Jump();
            
            // 1 - If current instruction is a jump, check current PC and update args
            if(jump){
                idx = i;
                passed = InsObjPtrs[i]->CheckResult(cpu, mips_Success, &msg);
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
            int tmpJumpPassed = InsObjPtrs[idx]->CheckResult(cpu, mips_Success, &msg);
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
                int passed = InsObjPtrs[i]->CheckResult(cpu, err, &msg);

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
            int passed = InsObjPtrs[i]->CheckResult(cpu, err, &msg);

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
    
        int passed = InsObjPtrs[i]->CheckResult(cpu, err, &msg);
    
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
                cout<<"once"<<endl;
                idx = i;
                passed = InsObjPtrs[i]->CheckResult(cpu, mips_Success, &msg);
                jumpPassed = passed;            
            }
        }

        // 2 - put register values in cpu
        InsObjPtrs[i]->SetRegs(cpu);
    
        // 3 - step CPU
        err = mips_cpu_step(cpu);

        // 4 - Set pc manually if step returned an exception or failed
        if(err){
            mips_error errtmp = mips_cpu_set_pc(cpu, pc+4);
            if(errtmp) cout<<" Could not set pc after step failed"<<endl;
        }

        
        // 5 - If Jump is currenlty being tested, check result
        if(jump){ 
            // PASS THE PROPER ERROR CODE TO THE CHECK RESULT, MSG IS FINE, GETS UPDATED
                cout<<"twice"<<endl;
            
            // 1 - Check the result
            int tmpJumpPassed = InsObjPtrs[idx]->CheckResult(cpu, mips_Success, &msg);
            jumpPassed = tmpJumpPassed & jumpPassed;

            // 2 - Check whether testing is over
            jump = InsObjPtrs[idx]->get_Jump();
            
            if(jump==0){
                cout<<"end"<<endl;

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
            

            }
        }
        
        //testId = 5;
        
        // 6 - If Jump Testing has ended, find out whether current instruction is a Jump
        if(jump==0){
            
            // 7 -Check the result
            int passed = InsObjPtrs[i]->CheckResult(cpu, err, &msg);

            // get_Jump returns 0 after it's last test has finished, hence if only 1 test is required
            // it is not a jump instruction
            // 8 - Check if current instruction is a Jump
            jump = InsObjPtrs[i]->get_Jump();
            
            // 9 - If current instruction is a Jump, update values
            if(jump){
                idx = i;
                //testIdTmp = testId;
                jumpPassed = passed;
            }

            // 10 - If not a Jump, Finish test
            else{
                // Print Ins Obj if instruction failed
                if(!passed){
                    cout<<"Following instruction failed"<<endl;
                    InsObjPtrs[i]->printInsObj(dest, err);
                } 

                // 11 - Record Current Instruction Test
                testId = mips_test_begin_test(InsObjPtrs[i]->get_name());    
                mips_test_end_test(testId, passed, msg);
            }
            
        }

        // 6 - If Jump Testing has not ended, Finish current test
        else{
            // 7 -Check the result
            int passed = InsObjPtrs[i]->CheckResult(cpu, err, &msg);

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