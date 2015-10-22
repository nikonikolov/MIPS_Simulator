#ifndef INSRCSV_H
#define INSRCSV_H

#include "../InsCSV/InsCSV.h"

class InsRCSV : public InsCSV {

public:
	
	InsRCSV(char* nameIn, uint8_t opcodeIn =0, uint8_t rsIn, uint8_t rtIn, uint8_t rdIn, uint8_t shiftIn =0, uint8_t fnIn,
			 int src1In, int src2In, int resultIn);

	uint32_t Build();
	int CheckResult(mips_cpu_h cpuPtr, char** msg =NULL);
	void SetRegs(mips_cpu_h cpuPtr);


	
private:
	uint8_t rs;
	uint8_t rt;
	uint8_t rd;
	uint8_t shift;
	uint8_t fn;

	int src1;
	int src2;
	int result;

};

#endif

