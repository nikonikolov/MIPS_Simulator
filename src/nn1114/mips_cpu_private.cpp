#include "mips_cpu_private.h"

uint32_t change_endian(uint32_t word){
	 return (word << 24) |
          ((word <<  8) & 0x00ff0000) |
          ((word >>  8) & 0x0000ff00) |
          ((word >> 24) & 0x000000ff);
}
