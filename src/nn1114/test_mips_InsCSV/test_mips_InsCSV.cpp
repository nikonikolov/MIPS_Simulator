#include "test_mips_InsCSV.h"

//InsCSV::InsCSV(){}

InsCSV::InsCSV(string name_in, uint8_t opcode_in, uint16_t exception_in, string msg_in, uint8_t Jump_in /*=0*/):
				name(name_in), opcode(opcode_in), exception(exception_in), msg(msg_in), Jump(Jump_in) {
					JumpTmp=Jump;
				}


char* InsCSV::get_name(){
	return &(name[0]);
}

char* InsCSV::get_msg(){
	return &(msg[0]);
}

uint8_t InsCSV::get_Jump(){
	// If Jump Check is over, reset JumpTmp to allow for second pass through this instruction
	if(JumpTmp==0){
		JumpTmp=Jump;
		return 0;
	}

	return JumpTmp;
}

