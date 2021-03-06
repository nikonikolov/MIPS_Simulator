#include "mips_cpu_ins_set.h"

// NOTE: 0x80000000 returns 0x80000000
static uint32_t twoscomp(uint32_t number){
	return ((0xFFFFFFFF^number) + 1);
}

static uint64_t twoscomp64(uint64_t number){
	return ((0xFFFFFFFFFFFFFFFF^number) + 1);
}

static uint32_t zero_extend(uint32_t arg){
	return 	(0x0000FFFF & arg);
}

// Returns true if src1<src2 and false otherwise
static bool compSigned(uint32_t src1, uint32_t src2){
	bool s1_neg = 0x80000000 & src1;
	bool s2_neg = 0x80000000 & src2;
	
	// src1 and src2 both negative
	if(s1_neg && s2_neg){
		// compare unsigned
		if(src2>src1) return true;
		else return false;
	}

	// src1 and/or src2 is positive
	if(s1_neg) return true;

	// src1 is 0 or positive, src2 negative
	if(s2_neg) return false;

	// both positive
	if(src1<src2) return true;

	// src1>=src2
	return false;
}

// Returns true if arg>0 and false otherwise
static bool GreaterThanZero(uint32_t arg){
	
	// arg<=0
	if( (0x80000000 & arg) || arg==0) return false;

	// arg>0
	return true;
}

// Returns true if arg>=0 and false otherwise
static bool GreaterThanEqualZero(uint32_t arg){
	
	// arg<0
	if(0x80000000 & arg) return false;

	// arg>=0
	return true;
}

DEFR(add){
	
	mips_error err = argzerocheck(shift);
	if(err) return err;
	
	// do arithmetics
	result = src1 + src2;
	
	// check overflow
	err = check_overflow(src1, src2, result);
	if(err) return err;

	return mips_Success;
}


DEFR(addu){

    mips_error err = argzerocheck(shift);
	if(err) return err;

	// do arithmetics
	result = src1 + src2;
	
	return mips_Success;
}


DEFR(sub){
	mips_error err = argzerocheck(shift);
	if(err) return err;
	
	// do arithmetics
	result = src1 - src2;
	
	// check overflow
	err = check_sub_overflow(src1, src2, result);
	if(err) return err;

	return mips_Success;

	/*if(src2==0x80000000){
		mips_error err = argzerocheck(shift);
		if(err) return err;
		result = src1+src2;
		return mips_Success;
	} 

	return add(state, src1, twoscomp(src2), result, shift, rs, rt, rd, write);*/
}

DEFR(subu){
	return addu(state, src1, twoscomp(src2), result, shift, rs, rt, rd, write);
}

DEFR(andr){
    mips_error err = argzerocheck(shift);
	if(err) return err;

	// do arithmetics
	result = src1 & src2;

	return mips_Success; 
}

DEFR(orr){
    mips_error err = argzerocheck(shift);
	if(err) return err;

	// do arithmetics
	result = src1 | src2;

	return mips_Success; 
}

DEFR(xorr){
    mips_error err = argzerocheck(shift);
	if(err) return err;

	// do arithmetics
	result = src1 ^ src2;

	return mips_Success; 
}

DEFR(divr){ 
	write = false;								// make sure you don't change state of cpu after you go back to call

    mips_error err = argzerocheck(rd);
	if(err) return err;
    err = argzerocheck(shift);
	if(err) return err;

	if(src2==0) return mips_ExceptionBreak;

	bool src1_pos = GreaterThanEqualZero(src1);
	bool src2_pos = GreaterThanEqualZero(src2);

	uint32_t m1, m2;
	if(!src1_pos) m1 = (uint32_t)twoscomp(src1);
	else m1 = (uint32_t)src1;
    
	if(!src2_pos) m2 = (uint32_t)twoscomp(src2);
	else m2 = (uint32_t)src2;


	uint32_t quotient = (m1/m2);
	uint32_t remaind = m1%m2;

	if(!src1_pos){
		quotient++;
		if(src2_pos) remaind -= src2;
		else  remaind += src2;
		remaind = twoscomp(remaind);
	}
	
	if( (src1_pos ^ src2_pos) ) quotient = twoscomp(quotient);
	
	state->HI = remaind; 
	state->LO = quotient;

	return mips_Success; 
}


DEFR(divu){ 

	write = false;								// make sure you don't change state of cpu after you go back to call

    mips_error err = argzerocheck(rd);
	if(err) return err;
    err = argzerocheck(shift);
	if(err) return err;

	if(src2==0) return mips_ExceptionBreak;
	
	uint32_t quotient = (src1/src2);
	uint32_t remaind = src1%src2;

	state->HI = remaind; 
	state->LO = quotient;

	return mips_Success; 

}


DEFR(mult){ 
	write = false;								// make sure you don't change state of cpu after you go back to call

    mips_error err = argzerocheck(rd);
	if(err) return err;
    err = argzerocheck(shift);
	if(err) return err;

	bool src1_pos = GreaterThanEqualZero(src1);
	bool src2_pos = GreaterThanEqualZero(src2);

	uint64_t m1, m2;
	if(!src1_pos) m1 = (uint64_t)twoscomp(src1);
	else m1 = (uint64_t)src1;
    
	if(!src2_pos) m2 = (uint64_t)twoscomp(src2);
	else m2 = (uint64_t)src2;


	uint64_t product = (uint64_t)(m1*m2);

	if( (src1_pos ^ src2_pos) && product) product = twoscomp64(product);
	
	state->HI = (uint32_t)(uint64_t)(product>>32); 
	state->LO = (uint32_t)(product);

	return mips_Success; 
}

DEFR(multu){ 

	write = false;								// make sure you don't change state of cpu after you go back to call

    mips_error err = argzerocheck(rd);
	if(err) return err;
    err = argzerocheck(shift);
	if(err) return err;

	bool src1_pos = GreaterThanEqualZero(src1);
	bool src2_pos = GreaterThanEqualZero(src2);

	uint64_t m1, m2;
	if(!src1_pos) m1 = (uint64_t)twoscomp(src1);
	else m1 = (uint64_t)src1;
    
	if(!src2_pos) m2 = (uint64_t)twoscomp(src2);
	else m2 = (uint64_t)src2;


	// do arithmetics
	uint64_t product = (uint64_t)(m1*m2);

	state->HI = (uint32_t)(uint64_t)(product>>32);
	state->LO = (uint32_t)product;

	return mips_Success;  

}

DEFR(mfhi){ 
    mips_error err = argzerocheck(rs);
	if(err) return err;
    err = argzerocheck(rt);
	if(err) return err;
    err = argzerocheck(shift);
	if(err) return err;
	
	result = state->HI;

	return mips_Success; 
}

DEFR(mflo){ 
    mips_error err = argzerocheck(rs);
	if(err) return err;
    err = argzerocheck(rt);
	if(err) return err;
    err = argzerocheck(shift);
	if(err) return err;
	
	result = state->LO;

	return mips_Success; 
}

DEFR(mthi){ 
	write = false;								// make sure you don't change state of cpu after you go back to call

    mips_error err = argzerocheck(rt);
	if(err) return err;
    err = argzerocheck(rd);
	if(err) return err;
    err = argzerocheck(shift);
	if(err) return err;
	
	state->HI = src1;

	return mips_Success; 
}

DEFR(mtlo){ 
	write = false;								// make sure you don't change state of cpu after you go back to call
    
    mips_error err = argzerocheck(rt);
	if(err) return err;
    err = argzerocheck(rd);
	if(err) return err;
    err = argzerocheck(shift);
	if(err) return err;
	
	state->LO = src1;

	return mips_Success; 
}

DEFR(sll){ 
    mips_error err = argzerocheck(rs);
	if(err) return err;

	result = (src2 << shift);
	return mips_Success; 
}

DEFR(srl){ 
    mips_error err = argzerocheck(rs);
	if(err) return err;

	result = (src2 >> shift);
	return mips_Success; 
}

DEFR(sllv){ 
    mips_error err = argzerocheck(shift);
	if(err) return err;
	uint8_t arg = (0x0000001F & src1);

	result = (src2 << arg);
	return mips_Success; 
}

DEFR(srlv){ 
    mips_error err = argzerocheck(shift);
	if(err) return err;
	uint8_t arg = (0x0000001F & src1);

	result = (src2 >> arg);
	return mips_Success; 
}

DEFR(sra){ 
    mips_error err = argzerocheck(rs);
	if(err) return err;

	uint32_t mask = 0x00000000;
	
	if(0x80000000 & src2){
		for(uint8_t i=0; i<shift; i++){
			mask = mask | (0x80000000 >> i);
		}
	}

	result = mask | (src2 >> shift);
	
	return mips_Success; 
}

DEFR(srav){ 
    mips_error err = argzerocheck(shift);
	if(err) return err;
	uint8_t arg = (0x0000001F & src1);

	uint32_t mask = 0x00000000;
	
	if(0x80000000 & src2){
		for(uint8_t i=0; i<arg; i++){
			mask = mask | (0x80000000 >> i);
		}
	}

	result = mask | (src2 >> arg);
	
	return mips_Success; 
}


DEFR(jr){ 	

	write = false;								// make sure you don't change state of cpu after you go back to call

	mips_error err;
	err = argzerocheck(rt);
	if(err) return err;
	err = argzerocheck(rd);
	if(err) return err;
	err = argzerocheck(shift);
	if(err) return err;

	uint32_t PC;
	mips_cpu_get_pc(state, &PC);
	//if(src1 & 0x00000003) return mips_ExceptionInvalidAddress; 
		
	// Change CPU state, set the jump address
	nn1114_mips_cpu_set_branch(state, (src1 - (PC+4)) );		// target last 2 bits are already zero, no error can be received

	return mips_Success; 
}

DEFR(jalr){ 	

	mips_error err;
	err = argzerocheck(rt);
	if(err) return err;
	err = argzerocheck(shift);
	if(err) return err;

	// Link, no matter if the condition is true or false
	uint32_t PC;
	mips_cpu_get_pc(state, &PC);
	result = PC + 8;

	//if(src1 & 0x00000003) return mips_ExceptionInvalidAddress; 
		
	// Change CPU state, set the jump address
	nn1114_mips_cpu_set_branch(state, (src1 - (PC+4)) );		// target last 2 bits are already zero, no error can be received

	return mips_Success;		
}


DEFR(slt){ 
	
	mips_error err = argzerocheck(shift);
	if(err) return err;
	
	if(compSigned(src1, src2)) result = 1;
	else result =0;

	return mips_Success; 
}

DEFR(sltu){ 
	
	mips_error err = argzerocheck(shift);
	if(err) return err;
	
	if(src1<src2) result = 1;
	else result =0;
	
	return mips_Success; 
}

/* *********************** I-TYPE *********************** */ 


DEFI(addi){

	// do arithmetics
	result = src1 + imm;
	
	// check overflow
	mips_error err = check_overflow(src1, imm, result);
	if(err) return err;

	return mips_Success;
}


DEFI(addiu){

	// do arithmetics
	result = src1 + imm;
	
    return mips_Success;
}


DEFI(andi){
	// mask top 16bits with 0
	imm = zero_extend(imm);

	// do arithmetics
	result = src1 & imm;

	return mips_Success; 
}

DEFI(ori){
	// mask top 16bits with 0
	imm = zero_extend(imm);

	// do arithmetics
	result = src1 | imm;

	return mips_Success; 
}

DEFI(xori){
	// mask top 16bits with 0
	imm = zero_extend(imm);

	// do arithmetics
	result = src1 ^ imm;

	return mips_Success; 
}


DEFI(b){ 
/*	if(rd==0x01) return bgez(state, src1, imm, result, rs, rd);
	if(rd==0x11) return bgezal(state, src1, imm, result, rs, rd);
	if(rd==0x00) return bltz(state, src1, imm, result, rs, rd);
	if(rd==0x10) return bltzal(state, src1, imm, result, rs, rd);
	*/

	if(rd==0x01) return CALLI(bgez);
	if(rd==0x11) return CALLI(bgezal);
	if(rd==0x00) return CALLI(bltz);
	if(rd==0x10) return CALLI(bltzal);

	return mips_ErrorNotImplemented; 
}

DEFI(bltz){  
	write = false;								// make sure you don't change state of cpu after you go back to call
	
	if(!GreaterThanEqualZero(src1)){
		imm = (imm<<2);
		
		// Change CPU state, set the offset for PC
		nn1114_mips_cpu_set_branch(state, imm);		// imm last 2 bits are already zero, no error can be received
	}

	return mips_Success;
}

DEFI(bltzal){  
	write = false;								// make sure you don't change state of cpu after you go back to call
	
	uint32_t PC;
	mips_cpu_get_pc(state, &PC);

	// Link, no matter if the condition is true or false
	mips_cpu_set_register(state, 31, PC+8);
	if(!GreaterThanEqualZero(src1)){
		imm = (imm<<2);
		
		// Change CPU state, set the offset for PC
		nn1114_mips_cpu_set_branch(state, imm);		// imm last 2 bits are already zero, no error can be received
	}

	return mips_Success;
}



DEFI(bgez){  
	write = false;								// make sure you don't change state of cpu after you go back to call
	
	if(GreaterThanEqualZero(src1)){
		imm = (imm<<2);
		
		// Change CPU state, set the offset for PC
		nn1114_mips_cpu_set_branch(state, imm);		// imm last 2 bits are already zero, no error can be received
	}

	return mips_Success;
}

DEFI(bgezal){  
	write = false;								// make sure you don't change state of cpu after you go back to call
	
	uint32_t PC;
	mips_cpu_get_pc(state, &PC);

	// Link, no matter if the condition is true or false
	mips_cpu_set_register(state, 31, PC+8);
	if(GreaterThanEqualZero(src1)){
		imm = (imm<<2);
		
		// Change CPU state, set the offset for PC
		nn1114_mips_cpu_set_branch(state, imm);		// imm last 2 bits are already zero, no error can be received
	}

	return mips_Success;
}

DEFI(beq){  
	write = false;								// make sure you don't change state of cpu after you go back to call
	
	uint32_t src2;
	mips_cpu_get_register(state, rd, &src2);	// rd is already cut to 5 bits, no error can be received

	if(src1 == src2){
		imm = (imm<<2);
		
		// Change CPU state, set the offset for PC
		nn1114_mips_cpu_set_branch(state, imm);		// imm last 2 bits are already zero, no error can be received
	}

	return mips_Success;
}


DEFI(bne){  
	write = false;								// make sure you don't change state of cpu after you go back to call
	
	uint32_t src2;
	mips_cpu_get_register(state, rd, &src2);	// rd is already cut to 5 bits, no error can be received
	
	if(src1 != src2){
		imm = (imm<<2);
		
		// Change CPU state, set the offset for PC
		nn1114_mips_cpu_set_branch(state, imm);		// imm last 2 bits are already zero, no error can be received
	}

	return mips_Success;
}


DEFI(blez){  
	write = false;								// make sure you don't change state of cpu after you go back to call
	
	mips_error err = argzerocheck(rd);
	if(err) return err;
	
	if(!GreaterThanZero(src1)){
		imm = (imm<<2);
		
		// Change CPU state, set the offset for PC
		nn1114_mips_cpu_set_branch(state, imm);		// imm last 2 bits are already zero, no error can be received
	}

	return mips_Success;
}


DEFI(bgtz){  
	write = false;								// make sure you don't change state of cpu after you go back to call
	
	mips_error err = argzerocheck(rd);
	if(err) return err;
	
	if(GreaterThanZero(src1)){
		imm = (imm<<2);
		
		// Change CPU state, set the offset for PC
		nn1114_mips_cpu_set_branch(state, imm);		// imm last 2 bits are already zero, no error can be received
	}

	return mips_Success;
}

DEFI(lw){ 
	
	imm = imm + src1;
	if(imm & 0x00000003) return mips_ExceptionInvalidAddress;
	
	// Read from Memory  
    mips_error err = mips_mem_read(
        state->memPtr,                   //!< Handle to target memory
        imm,                         	 //!< Byte address to start transaction at
        4,                               //!< Number of bytes to transfer
        (uint8_t*)&(result)             	 //!< Receives the target bytes
    );

    // Change endian before you store in reg
	result = nn1114_change_endian(result);

    return mips_Success;  
}


DEFI(lh){ 
	
	uint32_t addr;

	imm = imm + src1;
	if(imm & 0x00000001) return mips_ExceptionInvalidAddress;
	
	// calculate aligned address
	addr = imm - imm%4;		 

	// Read from Memory  
    mips_mem_read(
        state->memPtr,                   //!< Handle to target memory
        addr,                         	 //!< Byte address to start transaction at
        4,                               //!< Number of bytes to transfer
        (uint8_t*)&(result)             	 //!< Receives the target bytes
    );

    // Construct the right word to write
    if(imm%4 == 0){
    	result = ( (result<<16) & 0xFFFF0000 );
    } 
    else{
    	result = ( result & 0xFFFF0000 );
    } 
	
	result = nn1114_change_endian(result);
    result = sign_extend((uint16_t)result);

    return mips_Success; 
}


DEFI(lb){ 
	
    uint32_t addr;

	imm = imm + src1;
	
	// calculate aligned address
	addr = imm - imm%4;		 

	// Read from Memory  
    mips_mem_read(
        state->memPtr,                   //!< Handle to target memory
        addr,                         	 //!< Byte address to start transaction at
        4,                               //!< Number of bytes to transfer
        (uint8_t*)&(result)             	 //!< Receives the target bytes
    );

    // Construct the right word to write
    for(int i=0; i<4; i++){
    	if(imm%4 != i) result = ( 0x000000FF & (result>> i*8) );
    }

	if(result & 0x00000080) result = (0xFFFFFF00 | result);

    return mips_Success; 
}

DEFI(lwl){ return mips_ErrorNotImplemented; }
DEFI(lwr){ return mips_ErrorNotImplemented; }

DEFI(lhu){	
	
	uint32_t addr;

	imm = imm + src1;
	if(imm & 0x00000001) return mips_ExceptionInvalidAddress;
	
	// calculate aligned address
	addr = imm - imm%4;		 

	// Read from Memory  
    mips_mem_read(
        state->memPtr,                   //!< Handle to target memory
        addr,                         	 //!< Byte address to start transaction at
        4,                               //!< Number of bytes to transfer
        (uint8_t*)&(result)             	 //!< Receives the target bytes
    );

    // Construct the right word to write
    if(imm%4 == 0){
    	result = ( (result<<16) & 0xFFFF0000 );
    } 
    else{
    	result = ( result & 0xFFFF0000 );
    } 
	
	result = nn1114_change_endian(result);

    return mips_Success; 
}


DEFI(lbu){     

	uint32_t addr;

	imm = imm + src1;
	
	// calculate aligned address
	addr = imm - imm%4;		 

	// Read from Memory  
    mips_mem_read(
        state->memPtr,                   //!< Handle to target memory
        addr,                         	 //!< Byte address to start transaction at
        4,                               //!< Number of bytes to transfer
        (uint8_t*)&(result)             	 //!< Receives the target bytes
    );

    // Construct the right word to write
    for(int i=0; i<4; i++){
    	if(imm%4 != i) result = ( 0x000000FF & (result>> i*8) );
    }

	if(result & 0x00000080) result = (0xFFFFFF00 | result);

    return mips_Success;  
}

DEFI(lui){ 

	mips_error err = argzerocheck(rs);
	if(err) return err;

	result = ( (imm <<16) & 0xFFFF0000);

    return mips_Success;  
}

DEFI(sw){ 
	write = false;								// make sure you don't change state of cpu after you go back to call
	
	imm = imm + src1;
	if(imm & 0x00000003) return mips_ExceptionInvalidAddress;
	
	uint32_t src2;
	mips_cpu_get_register(state, rd, &src2);	// rd is already cut to 5 bits, no error can be received

	// Change endian before you store to memory
	src2 = nn1114_change_endian(src2);

	// Load in Memory  
    mips_error err = mips_mem_write(
        state->memPtr,                   //!< Handle to target memory
        imm,                         	 //!< Byte address to start transaction at
        4,                               //!< Number of bytes to transfer
        (uint8_t*)&(src2)             	 //!< Receives the target bytes
    );

    return mips_Success; 
}


DEFI(sh){ 
	write = false;								// make sure you don't change state of cpu after you go back to call
	
	uint32_t src2, Word, addr;

	imm = imm + src1;
	if(imm & 0x00000001) return mips_ExceptionInvalidAddress;
	
	// calculate aligned address
	addr = imm - imm%4;		 

	mips_cpu_get_register(state, rd, &src2);	// rd is already cut to 5 bits, no error can be received

	// Get current Word in Memory  
    mips_mem_read(
        state->memPtr,                   //!< Handle to target memory
        addr,                         	 //!< Byte address to start transaction at
        4,                               //!< Number of bytes to transfer
        (uint8_t*)&(Word)             	 //!< Receives the target bytes
    );

    // Change endianness before writing to mem
    src2 = ( (src2 << 8) & 0x0000FF00)  | ( (src2 >> 8) & 0x000000FF) ;
    
    // Construct the right word to write
    if(imm%4 == 0) Word = (src2 | (Word & 0xFFFF0000));
    else Word = ( (src2<<16) | (Word & 0x0000FFFF));
    

	// Load in Memory  
    mips_mem_write(
        state->memPtr,                   //!< Handle to target memory
        addr,                         	 //!< Byte address to start transaction at
        4,                               //!< Number of bytes to transfer
        (uint8_t*)&(Word)             	 //!< Receives the target bytes
    );

    return mips_Success; 
}

DEFI(sb){ 
	write = false;								// make sure you don't change state of cpu after you go back to call
	
	uint32_t src2, Word, mask=0x00000000, addr;

	imm = imm + src1;
	addr = imm - imm%4;		// calculate aligned address 

	mips_cpu_get_register(state, rd, &src2);	// rd is already cut to 5 bits, no error can be received

	// Get current Word in Memory  
    mips_mem_read(
        state->memPtr,                   //!< Handle to target memory
        addr,                         	 //!< Byte address to start transaction at
        4,                               //!< Number of bytes to transfer
        (uint8_t*)&(Word)             	 //!< Receives the target bytes
    );

    for(int i=0; i<4; i++){
    	if(imm%4 != i) mask = (mask | (Word & (0x000000FF<<i*8)));
    	else mask = (mask | ((src2 & 0x000000FF) <<i*8));
    }

	// Load in Memory  
    mips_mem_write(
        state->memPtr,                   //!< Handle to target memory
        addr,                         	 //!< Byte address to start transaction at
        4,                               //!< Number of bytes to transfer
        (uint8_t*)&(mask)             	 //!< Receives the target bytes
    );

    return mips_Success; 
}


DEFI(slti){ 
	
	if(compSigned(src1, imm)) result = 1;
	else result =0;

	return mips_Success; 
}

DEFI(sltiu){ 
	
	if(src1<imm) result = 1;
	else result =0;
	
	return mips_Success; 
}

/* *********************** J-TYPE *********************** */ 

DEFJ(j){ 	

	uint32_t PC;
	mips_cpu_get_pc(state, &PC);

	uint32_t target = ( ((PC+4) & 0xF0000000) | (0x0FFFFFFF & (arg<<2)) );
		
	// Change CPU state, set the jump address
	nn1114_mips_cpu_set_branch(state, (target - (PC+4)) );		// target last 2 bits are already zero, no error can be received

	return mips_Success; 
}
DEFJ(jal){ 	

	uint32_t PC;
	mips_cpu_get_pc(state, &PC);

	// Link, no matter if the condition is true or false
	mips_cpu_set_register(state, 31, PC+8);
	
	uint32_t target = ( ((PC+4) & 0xF0000000) | (0x0FFFFFFF & (arg<<2)) );
		
	// Change CPU state, set the jump address
	nn1114_mips_cpu_set_branch(state, (target - (PC+4)) );		// target last 2 bits are already zero, no error can be received

	return mips_Success; 
}
