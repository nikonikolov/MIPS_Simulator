#include "InsI.h"

InsI::InsI(uint8_t opcode_in, FPI FnImpl_in) :
	opcode(opcode_in), FnImpl(FnImpl_in) {}


FPI InsI::get_FnImpl() const{
	return FnImpl;
}


uint8_t	InsI::get_opcode() const{
	return opcode;
}