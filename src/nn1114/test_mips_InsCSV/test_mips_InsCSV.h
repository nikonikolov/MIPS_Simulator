#ifndef INSCSV_H
#define INSCSV_H

#include "mips.h"
#include "../test_mips_exitcheck.h"
#include "../test_mips_print.h"
#include "../test_mips_endian.h"

#include <string>

using namespace std;

#define CONINSCSV 		InsCSV(nameIn, opcodeIn, resultIn, exceptionIn, msgIn)
#define CONINSCSVJUMP 	InsCSV(nameIn, opcodeIn, resultIn, exceptionIn, msgIn, JumpIn, LinkIn)

class InsCSV{

public:

	InsCSV(string nameIn, uint8_t opcodeIn, uint32_t resultIn, uint16_t exceptionIn, string msgIn);

	InsCSV(string nameIn, uint8_t opcodeIn, uint32_t resultIn, uint16_t exceptionIn, string msgIn, 
			int JumpIn, bool LinkIn);

	virtual ~InsCSV(){}

	virtual uint32_t Build() =0;
	virtual int CheckResult(mips_cpu_h cpuPtr, mips_mem_h mem, mips_error excep_got, char** msg) =0;
	virtual void SetRegs(mips_cpu_h cpuPtr) =0;
	virtual void printInsObj(FILE *dest, mips_error err) =0;

//	char* get_name() const;
//	char* get_msg() const;

	char* get_name();
	char* get_msg();
	int get_Jump();

protected:
	string 		name;
	uint8_t 	opcode;
	uint16_t 	exception;
	uint32_t 	result;				// Expected Result
	uint32_t 	calcResult;			// Actual Result
	int 		Jump;				// 0 if not a jump. Otherwise, reduced to zero after the last test has been performed
	int 		JumpTmp;			// used to count how many steps are left till the end of the jump
	bool	 	Link;
	
	uint32_t	reg31;				// Hold value for reg31 before branch that does not link

	string 		msg;				// hint-error message for an instructions
};

#endif

