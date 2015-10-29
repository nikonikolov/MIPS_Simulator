#ifndef INSCSV_H
#define INSCSV_H

#include "mips.h"
#include "../test_mips_exitcheck.h"
#include "../test_mips_print.h"

#include <string>

class InsCSV{

public:

	//InsCSV();
	InsCSV(string name_in, uint8_t opcode_in, uint16_t exception_in, string msg_in, uint8_t Jump_in =0);

	virtual ~InsCSV(){}

	virtual uint32_t Build() =0;
	virtual int CheckResult(mips_cpu_h cpuPtr, mips_error excep_got, char** msg) =0;
	virtual void SetRegs(mips_cpu_h cpuPtr) =0;
	virtual void printInsObj(FILE *dest, mips_error err) =0;

//	char* get_name() const;
//	char* get_msg() const;

	char* get_name();
	char* get_msg();
	uint8_t get_Jump();

protected:
	string name;
	uint8_t opcode;
	uint16_t exception;
	uint32_t calcResult;
	uint8_t Jump;				// 0 if not a jump. Otherwise, reduced to zero after the last test has been performed
	uint8_t JumpTmp;			// used to count how many steps are left till the end of the jump
	string msg;					// hint-error message for an instructions
};

#endif

