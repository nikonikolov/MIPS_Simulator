#ifndef INSRCSV_H
#define INSRCSV_H

#include "../InsCSV/InsCSV.h"

class InsRCSV : public InsCSV {

public:
	
	InsRCSV(char* nameIn, uint8_t opcodeIn, uint8_t rsIn, uint8_t rtIn, uint8_t rdIn, uint8_t shiftIn, uint8_t fnIn,
			 uint32_t src1In, uint32_t src2In, uint32_t resultIn, char* msgIn);

	~InsRCSV(){}

	uint32_t Build();
	int CheckResult(mips_cpu_h cpuPtr, char** msg);
	void SetRegs(mips_cpu_h cpuPtr);
	void printInsObj(mips_cpu_h state);

	
private:
	uint8_t rs;
	uint8_t rt;
	uint8_t rd;
	uint8_t shift;
	uint8_t fn;

	uint32_t src1;
	uint32_t src2;
	uint32_t result;

};

#endif

