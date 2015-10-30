#include "test_mips_print.h"

/* *********************** HELP FUNCTIONS NEEDED ONLY IN THE FUNCTIONS IN THE CORRESPONDING HEADER *********************** */ 

static void printBin(FILE *dest, uint32_t InsWord, char* id){
	
	if(id!=NULL) fprintf(dest, "%s: ", id);
	
	fprintf(dest, "BINARY ");
	for(int i=0; i<32; i++){
    	if (InsWord<<i & 0x80000000) fprintf(dest, "1");
    	else fprintf(dest, "0");
    	if(i%4==3) fprintf(dest, " ");
	}
	fprintf(dest, "\n");
}



/* *********************** DEBUGGING AND PRINTING *********************** */ 


mips_error printPC(mips_cpu_h state, FILE *dest){
	uint32_t PC, nPC;
	
	mips_cpu_get_pc (state, &PC);
	nn1114_mips_cpu_get_npc (state, &nPC);
	
	debugPrintWord(dest, PC, "PC");
	debugPrintWord(dest, nPC, "nPC");
	return mips_Success;
}


void debugPrintWord(FILE *dest, const uint32_t& InsWord, char* id /*=NULL*/){
	if(id!=NULL) fprintf(dest, "%s: ", id);
	fprintf(dest, "HEX 0x%x\n", InsWord);
	printBin(dest, InsWord, id);
	fprintf(dest, "\n");
}

mips_error printErr(FILE *dest, mips_error err, char* msg){
		fprintf(dest, "%s\n", msg);
		fprintf(dest, "Error code %x\n", err);
	return err;	
}

