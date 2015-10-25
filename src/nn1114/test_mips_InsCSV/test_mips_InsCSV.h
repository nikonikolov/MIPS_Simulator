#ifndef INSCSV_H
#define INSCSV_H

#include "mips.h"
#include "../test_mips_exitcheck.h"
#include "../common_print.h"

#include <string>

class InsCSV{

public:

	//InsCSV();
	InsCSV(string name_in, uint8_t opcode_in, uint16_t exception_in, string msg_in = NULL);

	virtual ~InsCSV(){}

	virtual uint32_t Build() =0;
	virtual int CheckResult(mips_cpu_h cpuPtr, mips_error excep_got, char** msg) =0;
	virtual void SetRegs(mips_cpu_h cpuPtr) =0;
	virtual void printInsObj(mips_cpu_h state) =0;

//	char* get_name() const;
//	char* get_msg() const;

	char* get_name();
	char* get_msg();

protected:
	string name;
	uint8_t opcode;
	uint16_t exception;
	string msg;		// optional message for an instructions
};

#endif

