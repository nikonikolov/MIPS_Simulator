#ifndef INSCSV_H
#define INSCSV_H

#include "mips.h"
#include "../test_mips_exitcheck.h"
#include "../common_print.h"

class InsCSV{

public:

	//InsCSV();
	InsCSV(char* name_in, uint8_t opcode_in, char* msg_in = NULL);

	virtual ~InsCSV(){}

	virtual uint32_t Build() =0;
	virtual int CheckResult(mips_cpu_h cpuPtr, char** msg) =0;
	virtual void SetRegs(mips_cpu_h cpuPtr) =0;
	virtual void printInsObj(mips_cpu_h state) =0;

	char* get_name() const;
	char* get_msg() const;

protected:
	char* name;
	uint8_t opcode;
	char* msg;		// optional message for an instructions
};

#endif

