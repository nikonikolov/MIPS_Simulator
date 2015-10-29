#ifndef Jcsv_H
#define Jcsv_H

#include "../test_mips_InsCSV/test_mips_InsCSV.h"

class Jcsv : public InsCSV {

public:
	
	Jcsv(string nameIn, uint8_t opcodeIn, uint32_t argIn,
			 uint32_t resultIn, uint16_t exceptionIn, uint8_t JumpIn, string msgIn);

	~Jcsv(){}

	uint32_t Build();
	int CheckResult(mips_cpu_h cpuPtr, mips_error excep_got, char** msg);
	void SetRegs(mips_cpu_h cpuPtr);
	void printInsObj(FILE *dest, mips_error err);

	
private:
	uint32_t arg;

	uint32_t result;
};

#endif

