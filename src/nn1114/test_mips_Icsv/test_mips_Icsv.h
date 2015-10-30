#ifndef Icsv_H
#define Icsv_H

#include "../test_mips_InsCSV/test_mips_InsCSV.h"

class Icsv : public InsCSV {

public:
	
	// Non-Jump Constructor - 9 arguments
	Icsv(string nameIn, uint8_t opcodeIn, uint8_t rsIn, uint8_t rdIn, uint16_t immIn,
			 uint32_t src1In, uint32_t resultIn, uint16_t exceptionIn, string msgIn);


	// Jump Constructor - 13 arguments
	Icsv(string nameIn, uint8_t opcodeIn, uint8_t rsIn, uint8_t rdIn, uint16_t immIn,
             uint32_t src1In, uint32_t src2In, int JumpIn, bool JumpTakeIn, bool LinkIn,
             uint32_t resultIn, uint16_t exceptionIn,  string msgIn);

	~Icsv(){}

	uint32_t Build();
	int CheckResult(mips_cpu_h cpuPtr, mips_error excep_got, char** msg);
	void SetRegs(mips_cpu_h cpuPtr);
	void printInsObj(FILE *dest, mips_error err);
	
private:
	int JumpTaken(mips_cpu_h cpuPtr, mips_error excep_got);
	int JumpNotTaken(mips_cpu_h cpuPtr, mips_error excep_got);


	uint8_t 		rs;
	uint8_t 		rd;
	uint16_t 		imm;

	uint32_t 		src1;				// value for register rs
	uint32_t 		src2;				// If jump, value for register rd

	bool 			JumpTake;

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

