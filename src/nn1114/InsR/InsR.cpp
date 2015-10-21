#include "InsR.h"

InsR::InsR(uint8_t opcode_in, FPR FnImpl_in) :
	opcode(opcode_in), FnImpl(FnImpl_in) {}


FPR InsR::get_FnImpl() const{
	return FnImpl;
}


uint8_t	InsR::get_opcode() const{
	return opcode;
}