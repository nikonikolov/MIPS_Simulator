#ifndef INSJCSV_H
#define INSJCSV_H

#include "../InsCSV/InsCSV.h"

class InsJCSV : public InsCSV {

public:
	
	InsJCSV(string nameIn, uint8_t opcodeIn, uint32_t argIn,
			 uint32_t resultIn, string msgIn);

	~InsJCSV(){}

	uint32_t Build();
	int CheckResult(mips_cpu_h cpuPtr, char** msg);
	void SetRegs(mips_cpu_h cpuPtr);
	void printInsObj(mips_cpu_h state);

	
private:
	uint32_t arg;

	uint32_t result;
};

#endif

