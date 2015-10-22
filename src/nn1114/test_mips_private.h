#ifndef test_mips_private_h
#define test_mips_private_h

uint32_t BuildR(uint32_t opcode =0, uint32_t rs, uint32_t rt, uint32_t rd, uint32_t shift, uint32_t fn);
uint32_t BuildI(uint32_t opcode, uint32_t rs, uint32_t rd, uint32_t imm);
uint32_t BuildJ(uint32_t opcode, uint32_t arg);



#endif

