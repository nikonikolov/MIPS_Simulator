#include "common_print.h"

/* *********************** HELP FUNCTIONS NEEDED ONLY IN THE FUNCTIONS IN THE CORRESPONDING HEADER *********************** */ 

static void printBin(mips_cpu_h state, uint32_t InsWord, char* id){
	
	if(id!=NULL) fprintf(state->logDst, "%s: ", id);
	
	fprintf(state->logDst, "BINARY ");
	for(int i=0; i<32; i++){
    	if (InsWord<<i & 0x80000000) fprintf(state->logDst, "1");
    	else fprintf(state->logDst, "0");
    	if(i%4==3) fprintf(state->logDst, " ");
	}
	fprintf(state->logDst, "\n");
}



/* *********************** DEBUGGING AND PRINTING *********************** */ 


mips_error printPC(mips_cpu_h state){
	uint32_t PC, nPC;
	
	mips_cpu_get_pc (state, &PC);
	mips_cpu_get_npc (state, &nPC);
	
	debugPrintWord(state, PC, "PC");
	debugPrintWord(state, nPC, "nPC");
	return mips_Success;
}


void debugPrintWord(mips_cpu_h state, const uint32_t& InsWord, char* id /*=NULL*/){
	if(id!=NULL) fprintf(state->logDst, "%s: ", id);
	fprintf(state->logDst, "HEX 0x%x\n", InsWord);
	printBin(state, InsWord, id);
	fprintf(state->logDst, "\n");
}

mips_error printErr(mips_cpu_h state, mips_error err, char* msg){
	if(state->logLevel>=LOGERROR){
		fprintf(state->logDst, "%s\n", msg);
		fprintf(state->logDst, "Error code %x\n", err);
	}
	return err;	
}

