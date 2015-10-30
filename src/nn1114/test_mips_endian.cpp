#include "test_mips_endian.h"

uint32_t test_nn1114_change_endian(uint32_t word){
     return ((word << 24) & 0xff000000) |
            ((word <<  8) & 0x00ff0000) |
            ((word >>  8) & 0x0000ff00) |
            ((word >> 24) & 0x000000ff);
}