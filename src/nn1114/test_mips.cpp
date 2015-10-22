#include "mips.h"
#include "test_mips_private.h"

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
    mips_mem_h mem=mips_mem_create_ram(4096, 4);
    
    mips_cpu_h cpu=mips_cpu_create(mem);
    
    mips_error err=mips_cpu_set_debug_level(cpu, debuglvl, stderr);
    checkDebug(err);
    
    mips_test_begin_suite();


    int testId = mips_test_begin_test("addu");    
    
    // 1 - Setup an instruction in ram
    // addu r3, r4, r5
    uint32_t InsWord = BuildR(4,5,3,0, 0x21);
    
    InsWord = change_endian(InsWord);
      
    err = mips_mem_write(
        mem,	                         //!< Handle to target memory
        0,	                             //!< Byte address to start transaction at
        4,	                             //!< Number of bytes to transfer
        (uint8_t*)&(InsWord)	         //!< Receives the target bytes
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
    uint32_t result;

    err = mips_cpu_get_register(cpu, 3, &result);
    checkRegGet(err);
    
   
    int passed = result == 40+50;
    
    mips_test_end_test(testId, passed, "50 + 50 != 90");
    
    
    testId = mips_test_begin_test("addu");    
    
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
    
    mips_test_end_test(testId, passed, "r0 <> 0");
    
    
    mips_test_end_suite();
    
    return 0;
}
