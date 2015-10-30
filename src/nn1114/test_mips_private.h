#ifndef test_mips_private_h
#define test_mips_private_h

//#include "mips_cpu_def.h"
#include "mips.h"
#include "test_mips_InsCSV/test_mips_InsCSV.h"
#include "test_mips_Rcsv/test_mips_Rcsv.h"
#include "test_mips_Icsv/test_mips_Icsv.h"
#include "test_mips_Jcsv/test_mips_Jcsv.h"
#include "test_mips_exitcheck.h"
#include "test_mips_endian.h"
#include "test_mips_CSVRow/test_mips_CSVRow.h"


#include <fstream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

void readFile(string filename, vector<InsCSV*>& InsObjPtrs);

mips_error loadMem(mips_mem_h mem, vector<InsCSV*>& InsObjPtrs);
mips_error loadIns(mips_mem_h mem, uint32_t address, InsCSV* InsObj);

#endif

