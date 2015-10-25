#ifndef test_mips_private_h
#define test_mips_private_h

//#include "mips_cpu_def.h"
#include "mips.h"
#include "InsCSV/InsCSV.h"
#include "InsRCSV/InsRCSV.h"
#include "InsICSV/InsICSV.h"
#include "InsJCSV/InsJCSV.h"
#include "test_mips_exitcheck.h"
#include "test_mips_CSVRow/test_mips_CSVRow.h"
#include "common_end.h"


#include <fstream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

void readFile(string filename, vector<InsCSV*>& InsObjPtrs);
//void readFile(ifstream& infile, vector<InsCSV*>& InsObjPtrs);

mips_error loadMem(mips_mem_h mem, vector<InsCSV*>& InsObjPtrs);
mips_error loadIns(mips_mem_h mem, uint32_t address, InsCSV* InsObj);

#endif

