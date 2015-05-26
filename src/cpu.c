#include "common_instances.h"



/**
 * Carries out the fetch, decode, execute cycle
 * @param cpu Pointer to the cpu
 */
void
cpu_cycle(cpu *cpu ){

	uint32_t instr = 0;
	uint32_t pc;

	/* Program counter will act as a pointer to memory*/
	pc = cpu->pc;
	instr = memory_fetch_word(pc);
	cpu->pc = pc + 4;

}
