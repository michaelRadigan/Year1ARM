#ifndef CPU_H
#define CPU_H

#include "common_instances.h"


struct Cpu{
	
    uint32_t reg[14]; 
    /*A pointer to the array holding the 15 generaq; purpose registers*
     *we don't need them all but I'm including all 14 for completeness*/
    uint32_t pc;
    uint32_t cpsr;
    uint32_t fetched; /*this will hold the next instruction to be decoded*/
    
    /*this will probably change, just here as a placeholder really*/
    uint32_t decode; 	
};

/**
 * CPU registers
 */
typedef enum{
	R0 = 0x0,
	R1 = 0x1,
	R2 = 0x2,
	R3 = 0x3,
	R4 = 0x4,
	R5 = 0x5,
	R6 = 0x6,
	R7 = 0x7,
	R8 = 0x8,
	R9 = 0x9,
	R10 = 0xA,
	R11 = 0xB,
	R12 = 0xC,
	RSP = 0xD,
	RLR = 0xE,
	RPC = 0xF,
	CPSR = 0x10
}cpuReg;


/* Function prototypes */
void cpu_cycle(struct Cpu *cpu);

#endif
