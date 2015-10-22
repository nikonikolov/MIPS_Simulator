#include "test_mips_private.h"

/*uint32_t BuildR(uint32_t opcode /, uint32_t rs, uint32_t rt, uint32_t rd, uint32_t shift, uint32_t fn){
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
*/

/* ************************  MEMORY OPERATIONS ************************ */

// Load a vector of instructions in memory
mips_error loadMem(mips_mem_h mem, vector<InsCSV*>& InsObjPtrs){

    for(int i=0; i<InsObjPtrs.size(); i++){
        mips_error err = loadIns(mem, i*BLOCKSIZE - 1, InsObjPtrs[i]);
        if(!err) return err; 
    }

    return mips_Success;
}


// Load a single instruction in memory
mips_error loadIns(mips_mem_h mem, uint32_t address, InsCSV* InsObj){
    
    // 1 - Build an instruction
    uint32_t InsWord = InsObj->Build();
    
    InsWord = change_endian(InsWord);
      
    // Load in RAM  
    err = mips_mem_write(
        mem,                             //!< Handle to target memory
        address,                         //!< Byte address to start transaction at
        4,                               //!< Number of bytes to transfer
        (uint8_t*)&(InsWord)             //!< Receives the target bytes
    );

    return checkMemWrite(err);
} 

