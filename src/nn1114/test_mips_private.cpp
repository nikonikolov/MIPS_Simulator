#include "test_mips_private.h"

/* *********************** HELP FUNCTIONS NEEDED ONLY IN THE FUNCTIONS IN THE CORRESPONDING HEADER *********************** */ 

static uint32_t tohex(string number){
    uint32_t result;

    stringstream ss;
    ss << hex << number;
    ss >> result;

    return result;
}   

static uint32_t touint32(string number){
    uint32_t result;

    stringstream ss;
    ss << number;
    ss >> result;

    return result;
}

/*static uint16_t touint16(string number){
    uint16_t result;

    stringstream ss;
    ss << number;
    ss >> result;

    return result;
}*/ 
        

static void parseIns(const CSVRow& RowObj, vector<InsCSV*>& InsObjPtrs){
    int size = RowObj.size();
    InsCSV* InsCSVPtr;

    /*for(int i=0; i<RowObj.size();i++){
            cout<<RowObj[i]<<endl;
        }*/

    if(size==12){       // R-type instruction
        InsCSVPtr = new Rcsv(RowObj[0],  (uint8_t)(tohex(RowObj[1])), (uint8_t)(touint32(RowObj[2])), 
                                            (uint8_t)(touint32(RowObj[3])), (uint8_t)(touint32(RowObj[4])), 
                                            (uint8_t)(touint32(RowObj[5])), (uint8_t)(tohex(RowObj[6])),
                                            (uint32_t)(touint32(RowObj[7])), (uint32_t)(touint32(RowObj[8])), 
                                            (uint32_t)(touint32(RowObj[9])), (uint16_t)(tohex(RowObj[10])), RowObj[11] );
        InsObjPtrs.push_back(InsCSVPtr);
    }

    else if(size==9){   // I-type instruction
        InsCSVPtr = new Icsv(RowObj[0],  (uint8_t)(tohex(RowObj[1])), (uint8_t)(touint32(RowObj[2])), 
                                            (uint8_t)(touint32(RowObj[3])), (uint16_t)(touint32(RowObj[4])), 
                                            (uint32_t)(touint32(RowObj[5])), (uint32_t)(touint32(RowObj[6])),
                                            (uint16_t)(tohex(RowObj[7])), RowObj[8] );
        InsObjPtrs.push_back(InsCSVPtr);
    }

    else if(size==6){   // J-type instruction
        InsCSVPtr = new Jcsv(RowObj[0], (uint8_t)(tohex(RowObj[1])), (uint32_t)(touint32(RowObj[2])), 
                                        (uint32_t)(touint32(RowObj[3])), (uint16_t)(tohex(RowObj[4])), RowObj[5] );
        InsObjPtrs.push_back(InsCSVPtr);
    }

    else{
        if(size!=0){
            cout<<"INVALID INSTRUCTION"<<endl;
            for(int i=0; i<size; i++) cout<<RowObj[i];
            cout<<endl;
        }
    }
}

/*
static uint32_t change_endian(uint32_t word){
     return ((word << 24) & 0xff000000) |
            ((word <<  8) & 0x00ff0000) |
            ((word >>  8) & 0x0000ff00) |
            ((word >> 24) & 0x000000ff);
}
*/

/* ************************  READING FROM FILE ************************ */


/*void readFile(ifstream& infile, vector<InsCSV*>& InsObjPtrs){

    CSVRow RowObj;

    while(infile >> RowObj){
        parseIns(RowObj, InsObjPtrs);
    }

}*/


void readFile(string filename, vector<InsCSV*>& InsObjPtrs){

    ifstream infile;

    infile.open("Instructions.csv");
    //infile.open(filename);

    if(!infile.is_open()){
        cout<<"Could not open input file"<<endl;
        exit(1);
    }

    CSVRow RowObj;

    while(infile >> RowObj){
        parseIns(RowObj, InsObjPtrs);
    }

    infile.close();
}

/* ************************  MEMORY OPERATIONS ************************ */

// Load a vector of instructions in memory
mips_error loadMem(mips_mem_h mem, vector<InsCSV*>& InsObjPtrs){

    for(int i=0; i<InsObjPtrs.size(); i++){
        mips_error err = loadIns(mem, i*4, InsObjPtrs[i]);
        if(err) return err; 
    }

    return mips_Success;
}


// Load a single instruction in memory
mips_error loadIns(mips_mem_h mem, uint32_t address, InsCSV* InsObj){
    
    // 1 - Build an instruction
    uint32_t InsWord = InsObj->Build();
    
    InsWord = change_endian(InsWord);
      
    // Load in RAM  
    mips_error err = mips_mem_write(
        mem,                             //!< Handle to target memory
        address,                         //!< Byte address to start transaction at
        4,                               //!< Number of bytes to transfer
        (uint8_t*)&(InsWord)             //!< Receives the target bytes
    );

    checkMemWrite(err);
    
    return mips_Success;
} 


