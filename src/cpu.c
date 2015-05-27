#include "common_instances.h"

#define BITS_DATA_PROC_MASK           0x0C000000
#define BITS_MULT_PTRN_MASK_1         0x000000F0
#define BITS_MULT_PTRN_MASK_2         0x0FC00000
#define BITS_SINGLE_DATA_TRANS_MASK_1 0x00600000
#define BITS_SINGLE_DATA_TRANS_MASK_2 0x0C000000
#define BITS_BRANCH_MASK              0x0F000000
#define BITS_COND_MASK			      0xF0000000

#define FLAG_I_MASK					  0x02000000
#define FLAG_A_MASK 				  0x00200000
#define FLAG_P_MASK 				  0x01000000
#define FLAG_A_MASK                   0x00100000 
#define OPCODE_MASK					  0x01E00000
#define FLAG_S_MASK					  0x00100000
#define REG_1_MASK  				  0x000F0000
#define REG_2_MASK 					  0x0000F000
#define REG_3_MASK 					  0x00000F00
#define REG_4_MASK 					  0x0000000F	
#define OPERAND_MASK                  0x00000FFF 
#define OFFSET_1_MASK                 0x00000FFF 
#define OFFSET_2_MASK                 0x00FFFFFF

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
check_bits(uint32_t instr, uint32_t mask, int shift, uint32_t expected){
	
    uint32_t result = instr & mask;
	
	if ((result >> shift) == expected){
		return EXIT_SUCCESS;
	}
	else{
		return EXIT_FAILURE;
	}
}


uint32_t
extract_bits(uint32_t instr, uint32_t mask, uint32_t shift){
	uint32_t result = instr & mask;
	return (result >> shift); 
}

/**
 * Checks if instruction is Data Processing
 * @param instr The instruction word
 * @return EXIT_SUCCESS if all ok
 */
int
instr_data_proc(uint32_t instr){
	return check_bits(instr, BITS_DATA_PROC_MASK, 26, 0x00000000);
}


/**
 * Checks if instruction is Multiply
 * @param instr The instruction word
 * @return EXIT_SUCCESS if all ok
 */
int
instr_mult(uint32_t instr){
	return check_bits(instr, BITS_MULT_PTRN_MASK_1, 4, 0x00000009) &&
		   check_bits(instr, BITS_MULT_PTRN_MASK_2, 22, 0x00000000);
}



/**
 * Checks if instruction is Single Data Transfer
 * @param instr The instruction word
 * @return EXIT_SUCCESS if all ok
 */
int
instr_single_data_trans(uint32_t instr){
	return check_bits(instr, BITS_SINGLE_DATA_TRANS_MASK_1, 21, 0x00000000) &&
		   check_bits(instr, BITS_SINGLE_DATA_TRANS_MASK_2, 26, 0x00000001);
}


/**
 * Checks if instruction is Branch
 * @param instr The instruction word
 * @return EXIT_SUCCESS if all ok
 */
int
instr_branch(uint32_t instr){
	return check_bits(instr, BITS_BRANCH_MASK, 24, 0x0000000A);
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
	return check_bits(instr, BITS_COND_MASK, 28, cpsr_reg);
}


/* Decoding of instructions */

void 
decode_data_proc(uint32_t instr){
	//get I flag
	uint32_t I_flag = extract_bits(instr, FLAG_I_MASK, 25);
	//get opcode
	uint32_t op_code = extract_bits(instr, OPCODE_MASK, 21); 
	//get s flag
	uint32_t S_flag = extract_bits(instr, FLAG_S_MASK, 20);
	//get Rn reg
	uint32_t rn_reg = extract_bits(instr, REG_1_MASK, 16);
	//get Rd reg
	uint32_t rd_reg = extract_bits(instr, REG_2_MASK, 12);
	//Operand2
	uint32_t operand_2 = extract_bits(instr, OPERAND_MASK, 0);
}

void
decode_mult(uint32_t instr){
	//get A flag
	uint32_t A_flag = extract_bits(instr, FLAG_A_MASK, 21);
	//get S flag
	uint32_t S_flag = extract_bits(instr, FLAG_S_MASK, 20);
	//get Rd reg
	uint32_t rd_reg = extract_bits(instr, REG_1_MASK, 16);
	//get Rn reg
	uint32_t rn_reg = extract_bits(instr, REG_2_MASK, 12);
	//get Rs reg
	uint32_t rs_reg = extract_bits(instr, REG_3_MASK, 8);
	//get Rm reg
	uint32_t rm_reg = extract_bits(instr, REG_4_MASK, 0);
}

void
decode_single_data_trans(uint32_t instr){
	//get I flag
	uint32_t I_flag = extract_bits(instr, FLAG_I_MASK, 25); 
	//get P flag
	uint32_t P_flag = extract_bits(instr, FLAG_P_MASK, 24);
	//get U flag
	uint32_t U_flag = extract_bits(instr, FLAG_U_MASK, 23);
	//get L flag
	uint32_t L_flag = extract_bits(instr, FLAG_L_MASK, 20);
	//get Rn flag
	uint32_t rn_reg = extract_bits(instr, REG_1_MASK, 16);
	//get Rd flag
	uint32_t rd_reg = extract_bits(instr, REG_2_MASK, 12);
	//get offset
	uint32_t offset = extract_bits(instr, OFFSET_1_MASK, 0);
}

void
decode_branch(uint32_t instr){
	//get offset
	uint32_t offset = extract_bits(instr, OFFSET_1_MASK, 0);

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
