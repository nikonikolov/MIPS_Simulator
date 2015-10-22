#include "mips.h"
 
/* *************************** MEM AND CPU MANIPULATIONS CHECK ********************* */

void checkWrap(mips_error err, char* msg);

void checkStep(mips_error err);

void checkDebug(mips_error err);

void checkMemWrite(mips_error err);

void checkMemRead(mips_error err);

void checkRegSet(mips_error err);

void checkRegGet(mips_error err);