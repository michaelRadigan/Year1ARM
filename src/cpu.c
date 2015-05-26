#include "common_instances.h"

#define BITS_DATA_PROC         2
#define BITS_MULT_PTRN_1       4
#define BITS_MULT_PTRN_2       6
#define BITS_SINGLE_DATA_TRANS 2
#define BITS_BRANCH   		   4


/* Boolean methods to check validity */


/**
 * Checks for the chosen bit pattern
 * @param instr The word instruction
 * @param offset Amount to shift bit-mask
 * @param shift  Amount to shift the extracted bits
 * @param expected Expected bit pattern
 * @return EXIT_SUCCESS if all ok
 */
int
get_bits(uint32_t instr, int offset, int shift, uint32_t expected){
	
	uint32_t mask_data_proc = (1 << offset) - 1;
    uint32_t result = instr & mask_data_proc;
	
	if ((result >> shift) == expected){
		return EXIT_SUCCESS;
	}
	else{
		return EXIT_FAILURE;
	}
}


/**
 * Checks if instruction is Data Processing
 * @param instr The word instruction
 * @return EXIT_SUCCESS if all ok
 */
int
instr_data_proc(uint32_t instr){
	return get_bits(instr, BITS_DATA_PROC, 26, 0x00000000);
}


/**
 * Checks if instruction is Multiply
 * @param instr The word instruction
 * @return EXIT_SUCCESS if all ok
 */
int
instr_mult(uint32_t instr){
	return get_bits(instr, BITS_MULT_PTRN_1, 4, 0x00000009) &&
		   get_bits(instr, BITS_MULT_PTRN_2, 22, 0x00000000);
}



/**
 * Checks if instruction is Single Data Transfer
 * @param instr The word instruction
 * @return EXIT_SUCCESS if all ok
 */
int
instr_single_data_trans(uint32_t instr){
	return get_bits(instr, BITS_SINGLE_DATA_TRANS, 21, 0x00000000) &&
		   get_bits(instr, BITS_SINGLE_DATA_TRANS, 26, 0x00000001);
}


/**
 * Checks if instruction is Branch
 * @param instr The word instruction
 * @return EXIT_SUCCESS if all ok
 */
int
instr_branch(uint32_t instr){
	return get_bits(instr, BITS_BRANCH, 24, 0x0000000A);
}


/* Execution of instructions */

void
execute_data_proc(uint32_t instr);


void
execute_mult(uint32_t instr);


void
execute_single_data_trans(uint32_t instr);

void
execute_branch(uint32_t instr);



void
instr_decode(uint32_t instr){
	
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
	else{
	
	}
}


/**
 * Carries out the fetch, decode, execute cycle
 * @param cpu Pointer to the cpu
 */
void
cpu_cycle(cpu *cpu){

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
	
	instr_decode(instr);
}


