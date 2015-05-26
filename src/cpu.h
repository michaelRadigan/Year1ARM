#ifndef CPU_H
#define CPU_H

#include "common_instances.h"


typedef struct{
	
	uint32_t *cpu;

	uint32_t decoded;
	uint32_t fetched;

	uint32_t r0;
	uint32_t r1;
	uint32_t r2;
	uint32_t r3;
	uint32_t r4;
	uint32_t r5;
	uint32_t r6;
	uint32_t r7;
	uint32_t r8;
	uint32_t r9;
	uint32_t r10;
	uint32_t r11;
	uint32_t r12;
	uint32_t rsp;
	uint32_t lr;
	uint32_t pc;
	uint32_t cpsr;
}cpu;


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
void cpu_cycle(cpu *cpu);

#endif
