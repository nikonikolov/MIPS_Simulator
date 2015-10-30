#ifndef Rcsv_H
#define Rcsv_H

#include "../test_mips_InsCSV/test_mips_InsCSV.h"

class Rcsv : public InsCSV {

public:
	
	// Non-Jump Constructor - 12 arguments	
	Rcsv(string nameIn, uint8_t opcodeIn, uint8_t rsIn, uint8_t rtIn, uint8_t rdIn, uint8_t shiftIn, uint8_t fnIn,
			 uint32_t src1In, uint32_t src2In, uint32_t resultIn, uint16_t exceptionIn, string msgIn);

	// Jump Constructor - 14 arguments
	Rcsv(string nameIn, uint8_t opcodeIn, uint8_t rsIn, uint8_t rtIn, uint8_t rdIn, uint8_t shiftIn, uint8_t fnIn,
        uint32_t src1In, uint32_t src2In, int JumpIn, bool LinkIn, uint32_t resultIn, uint16_t exceptionIn, string msgIn);

	~Rcsv(){}

	uint32_t Build();
	int CheckResult(mips_cpu_h cpuPtr, mips_mem_h mem, mips_error excep_got, char** msg);
	void SetRegs(mips_cpu_h cpuPtr);
	void printInsObj(FILE *dest, mips_error err);

	
private:
	int CheckJump(mips_cpu_h cpuPtr, mips_mem_h mem, mips_error excep_got);
	
	uint8_t rs;
	uint8_t rt;
	uint8_t rd;
	uint8_t shift;
	uint8_t fn;

	uint32_t src1;
	uint32_t src2;
	

	// ABSTRACT CLASS VARIABLES
	//string 		name;
	//uint8_t 		opcode;
	//uint16_t 		exception;
	//uint32_t 		result;				// Expected Result
	//uint32_t 		calcResult;			// Actual Result
	//int 			Jump;				// 0 if not a jump. Otherwise, reduced to zero after the last test has been performed
	//int 			JumpTmp;			// used to count how many steps are left till the end of the jump
	//bool	 		Link;
	//uint32_t		reg31;				// Hold value for reg31 before branch that does not link
	//string 		msg;				// hint-error message for an instructions
};

#endif

