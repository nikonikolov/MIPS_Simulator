#ifndef INSICSV_H
#define INSICSV_H

#include "../InsCSV/InsCSV.h"

class InsICSV : public InsCSV {

public:
	
	InsICSV(string nameIn, uint8_t opcodeIn, uint8_t rsIn, uint8_t rdIn, uint16_t immIn,
			 uint32_t src1In, uint32_t resultIn, string msgIn);

	~InsICSV(){}

	uint32_t Build();
	int CheckResult(mips_cpu_h cpuPtr, char** msg);
	void SetRegs(mips_cpu_h cpuPtr);
	void printInsObj(mips_cpu_h state);

	
private:
	uint8_t rs;
	uint8_t rd;
	uint16_t imm;

	uint32_t src1;
	uint32_t result;

};

#endif

