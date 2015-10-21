#include "InsJ.h"

InsJ::InsJ(uint8_t opcode_in, FPJ FnImpl_in) :
	opcode(opcode_in), FnImpl(FnImpl_in) {}


FPJ InsJ::get_FnImpl() const{
	return FnImpl;
}


uint8_t	InsJ::get_opcode() const{
	return opcode;
}