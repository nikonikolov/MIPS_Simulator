#include "Ins.h"

Ins::Ins (uint8_t opcode_in, char* name_in):
	opcode(opcode_in), name(name_in) {}


char* Ins::get_name(){
	return name;
}


uint8_t	Ins::get_opcode() const{
	return opcode;
}