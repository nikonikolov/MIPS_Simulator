#include "InsCSV.h"

//InsCSV::InsCSV(){}


InsCSV::InsCSV(string name_in, uint8_t opcode_in, string msg_in /*= NULL*/):
				name(name_in), opcode(opcode_in), msg(msg_in) {}


char* InsCSV::get_name(){
	return &(name[0]);
}

char* InsCSV::get_msg(){
	return &(msg[0]);
}
