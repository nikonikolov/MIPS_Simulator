#include "test_mips_InsCSV.h"

//InsCSV::InsCSV(){}

InsCSV::InsCSV(string name_in, uint8_t opcode_in, uint16_t exception_in, string msg_in, uint32_t calcResult_in /*=0*/):
				name(name_in), opcode(opcode_in), exception(exception_in), msg(msg_in), calcResult(calcResult_in) {}


char* InsCSV::get_name(){
	return &(name[0]);
}

char* InsCSV::get_msg(){
	return &(msg[0]);
}
