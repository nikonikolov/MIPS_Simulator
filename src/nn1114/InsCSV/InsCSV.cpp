#include "InsCSV.h"

//InsCSV::InsCSV(){}


InsCSV::InsCSV(char* name_in, uint8_t opcode_in, char* msg_in /*= NULL*/):
				name(name_in), opcode(opcode_in), msg(msg_in) {}


char* InsCSV::get_name() const{
	return name;
}

char* InsCSV::get_msg() const{
	return msg;
}
