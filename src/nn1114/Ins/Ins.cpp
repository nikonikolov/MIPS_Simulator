#include "Ins.h"

Ins::Ins(uint8_t opcode_in, FP FnImpl_in) :
	opcode(opcode_in), FnImpl(FnImpl_in) {}


FP Ins::get_FnImpl() const{
	return FnImpl;
}


uint8_t	Ins::get_opcode() const{
	return opcode;
}