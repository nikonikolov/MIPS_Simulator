#include "test_mips_InsCSV.h"

//InsCSV::InsCSV(){}

// Non-Jump Constructor
InsCSV::InsCSV(string nameIn, uint8_t opcodeIn, uint32_t resultIn, uint16_t exceptionIn, string msgIn) :
				
				name(nameIn), opcode(opcodeIn), result(resultIn), exception(exceptionIn), msg(msgIn) {
					Jump=0;
					JumpTmp=Jump;
					Link=false;
					if(result==0) calcResult=1;
                	else calcResult=0;
				}

// Jump Constructor
InsCSV::InsCSV(string nameIn, uint8_t opcodeIn, uint32_t resultIn, uint16_t exceptionIn, string msgIn, 
				int JumpIn, bool LinkIn) :
				
				name(nameIn), opcode(opcodeIn), result(resultIn), exception(exceptionIn), msg(msgIn), 
				Jump(JumpIn), Link(LinkIn) {
					JumpTmp=Jump;
					if(result==0) calcResult=1;
                	else calcResult=0;
				}


char* InsCSV::get_name(){
	return &(name[0]);
}

char* InsCSV::get_msg(){
	return &(msg[0]);
}

int InsCSV::get_Jump(){
	// If Jump Check is over, reset JumpTmp to allow for second pass through this instruction
	if(JumpTmp==0){
		JumpTmp=Jump;
		return 0;
	}

	return JumpTmp;
}

