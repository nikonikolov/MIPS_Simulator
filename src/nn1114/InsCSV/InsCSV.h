#ifndef INSCSV_H
#define INSCSV_H

#include "mips.h"
#include "../test_mips_checkman.h"

class InsCSV{

public:

	InsCSV(char* name_in, uint8_t opcode_in);


	virtual uint32_t Build() =0;
	virtual int CheckResult(mips_cpu_h cpuPtr, char** msg =NULL) =0;
	virtual void SetRegs(mips_cpu_h cpuPtr) =0;


	uint32_t twoscomplement(int NegNumber);

	char* get_name() const:

protected:
	char* name;
	uint8_t opcode;
};

#endif

