#ifndef Icsv_H
#define Icsv_H

#include "../test_mips_InsCSV/test_mips_InsCSV.h"

class Icsv : public InsCSV {

public:
	
	Icsv(string nameIn, uint8_t opcodeIn, uint8_t rsIn, uint8_t rdIn, uint16_t immIn,
			 uint32_t src1In, uint32_t resultIn, uint16_t exceptionIn, string msgIn);

	~Icsv(){}

	uint32_t Build();
	int CheckResult(mips_cpu_h cpuPtr, mips_error excep_got, char** msg);
	void SetRegs(mips_cpu_h cpuPtr);
	void printInsObj(mips_cpu_h state, mips_error err);

	
private:
	uint8_t rs;
	uint8_t rd;
	uint16_t imm;

	uint32_t src1;
	uint32_t result;

};

#endif

