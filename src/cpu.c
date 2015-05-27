#include "common_instances.h"

#define BITS_DATA_PROC_MASK           0x0C000000
#define BITS_MULT_PTRN_MASK_1         0x000000F0
#define BITS_MULT_PTRN_MASK_2         0x0FC00000
#define BITS_SINGLE_DATA_TRANS_MASK_1 0x00600000
#define BITS_SINGLE_DATA_TRANS_MASK_2 0x0C000000
#define BITS_BRANCH_MASK              0x0F000000
#define BITS_COND_MASK			      0xF0000000


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
get_bits(uint32_t instr, uint32_t mask, int shift, uint32_t expected){
	
    uint32_t result = instr & mask;
	
	if ((result >> shift) == expected){
		return EXIT_SUCCESS;
	}
	else{
		return EXIT_FAILURE;
	}
}


/**
 * Checks if instruction is Data Processing
 * @param instr The instruction word
 * @return EXIT_SUCCESS if all ok
 */
int instr_data_proc(uint32_t instr){
	return get_bits(instr, BITS_DATA_PROC_MASK, 26, 0x00000000);
}


/**
 * Checks if instruction is Multiply
 * @param instr The instruction word
 * @return EXIT_SUCCESS if all ok
 */
int
instr_mult(uint32_t instr){
	return get_bits(instr, BITS_MULT_PTRN_MASK_1, 4, 0x00000009) &&
		   get_bits(instr, BITS_MULT_PTRN_MASK_2, 22, 0x00000000);
}



/**
 * Checks if instruction is Single Data Transfer
 * @param instr The instruction word
 * @return EXIT_SUCCESS if all ok
 */
int
instr_single_data_trans(uint32_t instr){
	return get_bits(instr, BITS_SINGLE_DATA_TRANS_MASK_1, 21, 0x00000000) &&
		   get_bits(instr, BITS_SINGLE_DATA_TRANS_MASK_2, 26, 0x00000001);
}


/**
 * Checks if instruction is Branch
 * @param instr The instruction word
 * @return EXIT_SUCCESS if all ok
 */
int
instr_branch(uint32_t instr){
	return get_bits(instr, BITS_BRANCH_MASK, 24, 0x0000000A);
}


/**
 * Checks if Condition field is satisfied by CPSR register
 * @param instr The instruction word
 * @return EXIT_SUCCESS if all ok
 */
int 
check_instr_cond_code(uint32_t instr){
	/* Uninitialised cpu cos havent implemented malloc/calloc */

	cpu *cpu;
	uint32_t cpsr_reg = cpu->cpsr;
	return get_bits(instr, BITS_COND_MASK, 28, cpsr_reg);
}


/* Execution of instructions */


void
execute_data_proc(uint32_t instr){
	/* If cond field is satisfied by cpsr reg then execute */
	if(check_instr_cond_code(instr)){

		
		/* Check if the I (Immediate Operand) flag is set (1)*/
		if(   get_bits()  ){
			// If set then Operand2 is an immediate constant
		}
		else{
			/* it is a shifted register */
		}
	
	}
	else{
		/* Don't execute and fetch next instruction */
	}
}

void
execute_mult(uint32_t instr){
	/* If cond field is satisfied by cpsr reg then execute */
	if(check_instr_cond_code(instr)){
	
	}
	else{
		/* Don't execute and fetch next instruction */
	}
}


void
execute_single_data_trans(uint32_t instr){
	/* If cond field is satisfied by cpsr reg then execute */
	if(check_instr_cond_code(instr)){
	
	}
	else{
		/* Don't execute and fetch next instruction */
	}
}


void
execute_branch(uint32_t instr){
	/* If cond field is satisfied by cpsr reg then execute */
	if(check_instr_cond_code(instr)){
	
	}
	else{
		/* Don't execute and fetch next instruction */
	}
}


void
instr_decode(uint32_t instr){
	
	if(instr_data_proc(instr)){
//		decode_data_proc(instr);
		execute_data_proc(instr);
	}
	else if(instr_mult(instr)){
//		decode_mult(instr);
		execute_mult(instr);
	}
	else if(instr_single_data_trans(instr)){
//		decode_single_data_trans(instr);
		execute_single_data_trans(instr);
	}
	else if(instr_branch(instr){
//			decode_branch(instr);
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

	/* Before cpu struct pointer is passed in we need to initialise it */

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
