#ifndef Jcsv_H
#define Jcsv_H

#include "../test_mips_InsCSV/test_mips_InsCSV.h"

class Jcsv : public InsCSV {

public:
	
	// Jump Constructor - 8 arguments
	Jcsv(string nameIn, uint8_t opcodeIn, uint32_t argIn,
             int JumpIn, bool LinkIn, uint32_t resultIn, uint16_t exceptionIn, string msgIn);

	~Jcsv(){}

	uint32_t Build();
	int CheckResult(mips_cpu_h cpuPtr, mips_error excep_got, char** msg);
	void SetRegs(mips_cpu_h cpuPtr);
	void printInsObj(FILE *dest, mips_error err);

	
private:
	uint32_t arg;

	// ABSTRACT CLASS VARIABLES
	//string 		name;
	//uint8_t 		opcode;
	//uint16_t 		exception;
	//uint32_t 		result;				// Expected Result
	//uint32_t 		calcResult;			// Actual Result
	//int 			Jump;				// 0 if not a jump. Otherwise, reduced to zero after the last test has been performed
	//int 			JumpTmp;			// used to count how many steps are left till the end of the jump
	//bool	 		Link;
	//string 		msg;				// hint-error message for an instructions
};

#endif

