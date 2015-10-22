#ifndef test_mips_private_h
#define test_mips_private_h

#include "mips_cpu_def.h"
#include "InsCSV/InsCSV.h"
#include "InsRCSV/InsRCSV.h"
#include "InsICSV/InsICSV.h"
#include "InsJCSV/InsJCSV.h"
#include "test_mips_checkman.h"


#include <vector>

mips_error loadMem(mips_mem_h mem, vector<InsCSV*>& InsObjPtrs);
mips_error loadIns(mips_mem_h mem, uint32_t address, InsCSV* InsObj);


#endif

