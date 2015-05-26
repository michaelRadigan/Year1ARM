#include "common_instances.h"
#define DATA_PROC_BITS 2


/* Boolean methods to check validity */


int
instr_data_proc(uint32_t instr);

int
instr_mult(uint32_t instr);


int
instr_single_data_trans(uint32_t instr);


int
instr_branch(uint32_t instr);


/* Execution of instructions */

void
execute_data_proc(uint32_t instr);


void
execute_mult(uint32_t instr);


void
execute_single_data_trans(uint32_t instr);

void
execute_branch(uint32_t instr);


/**
 * Carries out the fetch, decode, execute cycle
 * @param cpu Pointer to the cpu
 */
void
cpu_cycle(cpu *cpu ){

	uint32_t instr = 0;
	uint32_t pc;

	/* Program counter will act as a pointer to memory*/
	/* Fetch one instruction from memory */
	pc = cpu->pc;
	instr = memory_fetch_word(pc);
	cpu->pc = pc + 4;

	/* Check if all-0 instruction first */

	if(instr == 0){
		/* Upon termination output the state of all the registers */
	}


    /* get 01111111111111111111 */

	uint32_t data_proc_mask = (1 << DATA_PROC_BITS) - 1;

	if ((instr & ) == (b & mask))

	
		
	/* Decode */
	if(instr_data_proc(instr)){
		execute_data_proc(instr);
	}
	else if(instr_mult(instr)){
		execute_mult(instr);
	}
	else if(instr_single_data_trans(instr)){
		execute_single_data_trans(instr);
	}
	else if(instr_branch(instr){
			execute_branch(instr);
	}		
}


