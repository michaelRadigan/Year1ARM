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
#define FLAG_U_MASK                   0x00800000 
#define OPCODE_MASK					  0x01E00000
#define FLAG_S_MASK					  0x00100000
#define FLAG_L_MASK					  0x00100000
#define REG_1_MASK  				  0x000F0000
#define REG_2_MASK 					  0x0000F000
#define REG_3_MASK 					  0x00000F00
#define REG_4_MASK 					  0x0000000F	
#define OPERAND_MASK                  0x00000FFF 
#define OFFSET_1_MASK                 0x00000FFF 
#define OFFSET_2_MASK                 0x00FFFFFF


/* Global variables */


/* Pointer definitions */
cpu *cpu_ptr;


/* Program counter will act as a pointer to memory*/
uint32_t pc = 0; 


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


/**
 * Extracts the relevant bit selection
 * @param instr The word instruction
 * @param mask  The relevant bit mask
 * @param shift The relevant shift amount
 * @return The extracted bits
 */
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
	uint32_t cpsr_reg = cpu_ptr->cpsr;
	return check_bits(instr, BITS_COND_MASK, 28, cpsr_reg);
}


/* Decoding of instructions */


/**
 * Extracts the relevant bits and sets them in the Data Proc struct
 * @param instr The instruction word
 */
void 
decode_data_proc(uint32_t instr){
	//get I flag
	uint32_t I_flag = extract_bits(instr, FLAG_I_MASK, 25);
	instr_data_proc_ptr->I_flag = I_flag;

	//get opcode
	uint32_t op_code = extract_bits(instr, OPCODE_MASK, 21); 
	instr_data_proc_ptr->op_code = op_code;
	
	//get s flag
	uint32_t S_flag = extract_bits(instr, FLAG_S_MASK, 20);
	instr_data_proc_ptr->S_flag = S_flag;
	
	//get Rn reg
	uint32_t rn_reg = extract_bits(instr, REG_1_MASK, 16);
	instr_data_proc_ptr->rn_reg = rn_reg;
	
	//get Rd reg
	uint32_t rd_reg = extract_bits(instr, REG_2_MASK, 12);
	instr_data_proc_ptr->rd_reg = rd_reg;
	
	//Operand2
	uint32_t operand_2 = extract_bits(instr, OPERAND_MASK, 0);
	instr_data_proc_ptr->operand_2 = operand_2;
}


/**
 * Extracts the relevant bits and sets them in the Data Proc struct
 * @param instr The instruction word
 */
void
decode_mult(uint32_t instr){
	//get A flag
	uint32_t A_flag = extract_bits(instr, FLAG_A_MASK, 21);
	instr_mult_ptr->A_flag = A_flag;

	//get S flag
	uint32_t S_flag = extract_bits(instr, FLAG_S_MASK, 20);
	instr_mult_ptr->S_flag = S_flag;	
	
	//get Rd reg
	uint32_t rd_reg = extract_bits(instr, REG_1_MASK, 16);
	instr_mult_ptr->rd_reg = rd_reg;
	
	//get Rn reg
	uint32_t rn_reg = extract_bits(instr, REG_2_MASK, 12);
	instr_mult_ptr->rn_reg = rn_reg;	
	
	//get Rs reg
	uint32_t rs_reg = extract_bits(instr, REG_3_MASK, 8);
	instr_mult_ptr->rs_reg = rs_reg;	
	
	//get Rm reg
	uint32_t rm_reg = extract_bits(instr, REG_4_MASK, 0);
	instr_mult_ptr->rm_reg = rm_reg;
}


/**
 * Extracts the relevant bits and sets them in the Data Proc struct
 * @param instr The instruction word
 */
void
decode_single_data_trans(uint32_t instr){
	//get I flag
	uint32_t I_flag = extract_bits(instr, FLAG_I_MASK, 25); 
	instr_single_data_trans_ptr->I_flag = I_flag;
	
	//get P flag
	uint32_t P_flag = extract_bits(instr, FLAG_P_MASK, 24);
	instr_single_data_trans_ptr->P_flag = P_flag;
	
	//get U flag
	uint32_t U_flag = extract_bits(instr, FLAG_U_MASK, 23);
	instr_single_data_trans_ptr->U_flag = U_flag;
	
	//get L flag
	uint32_t L_flag = extract_bits(instr, FLAG_L_MASK, 20);
	instr_single_data_trans_ptr->L_flag = L_flag;	
	
	//get Rn flag
	uint32_t rn_reg = extract_bits(instr, REG_1_MASK, 16);
    instr_single_data_trans_ptr->rn_reg = rn_reg;	
	
	//get Rd flag
	uint32_t rd_reg = extract_bits(instr, REG_2_MASK, 12);
	instr_single_data_trans_ptr->rd_reg = rd_reg;	
	
	//get offset
	uint32_t offset = extract_bits(instr, OFFSET_1_MASK, 0);
	instr_single_data_trans_ptr->offset = offset;
}


/**
 * Extracts the relevant bits and sets them in the Data Proc struct
 * @param instr The instruction word
 */
void
decode_branch(uint32_t instr){
	//get offset
	uint32_t offset = extract_bits(instr, OFFSET_1_MASK, 0);
	instr_branch_ptr->offset = offset;
}
 

/* Execution of instructions */


void
execute_data_proc(){
    		
}

void
execute_mult(){

}


void
execute_single_data_trans(){

}


void
execute_branch(){
	uint32_t offset = instr_branch_ptr->offset;

	/* Shift offset left 2 bits, 
	 * and sign extend to 32 bits
	 */
	uint32_t result = offset << 2;
	
	/* Add result to pc */
	pc += result; 

}


/**
 * Selects which instruction to decode
 * @param instr The instruction word to be decoded
 */
void
instr_decode(uint32_t instr){
	
	if(instr_data_proc(instr)){
		decode_data_proc(instr);
	}
	else if(instr_mult(instr)){
		decode_mult(instr);
	}
	else if(instr_single_data_trans(instr)){
		decode_single_data_trans(instr);
	}
	else if(instr_branch(instr)){
			decode_branch(instr);
	}
	else{
	
	}
}


/**
 * Selects which instruction to execute
 * @param instr The instruction word to be executed
 */
void
instr_execute(uint32_t instr){
	
	if(instr_data_proc(instr) && check_instr_cond_code(instr)){
		execute_data_proc();
	}
	else if(instr_mult(instr) && check_instr_cond_code(instr)){
		execute_mult();
	}
	else if(instr_single_data_trans(instr) && check_instr_cond_code(instr)){
		execute_single_data_trans();
	}
	else if(instr_branch(instr) && check_instr_cond_code(instr)){
        execute_branch();
	}
	else{
	
	}
}


/**
 * Carries out the fetch, decode, execute cycle
 * @param cpu Pointer to the cpu
 */
void
/* ARGUMENT NOT NECESSARY??? */
cpu_cycle(cpu *cpu){

	/* Before cpu struct pointer is passed in we need to initialise it */

	uint32_t instr = 0;
    pc = cpu_ptr->pc;
	/* Fetch one instruction from memory */
	instr = memory_fetch_word(pc);
	cpu->pc = pc + 4;

	/* Check if all-0 instruction first */

	if(instr == 0){
		/* Upon termination output the state of all the registers */
	}
	
	instr_decode(instr);
	instr_execute(instr);
}
