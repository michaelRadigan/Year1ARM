#ifndef CPU_H
#define CPU_H

#include "common_instances.h"


typedef struct{
	uint32_t *cpu;
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
	R13 = 0xD,
	R14 = 0xE,
	R15 = 0xF
}cpuReg;

#endif
