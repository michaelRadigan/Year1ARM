#include "cpu.h"

#define BITS_DATA_PROC_MASK           0x0C000000
#define BITS_MULT_PTRN_MASK_1         0x000000F0
#define BITS_MULT_PTRN_MASK_2         0x0FC00000
#define BITS_SINGLE_DATA_TRANS_MASK_1 0x00600000
#define BITS_SINGLE_DATA_TRANS_MASK_2 0x0C000000
#define BITS_BRANCH_MASK              0x0F000000
#define BITS_COND_MASK		      0xF0000000

#define FLAG_I_MASK		      0x02000000
#define FLAG_A_MASK 		      0x00200000
#define FLAG_P_MASK 		      0x01000000
#define FLAG_U_MASK                   0x00800000 
#define OPCODE_MASK		      0x01E00000
#define FLAG_S_MASK		      0x00100000
#define FLAG_L_MASK		      0x00100000
#define REG_1_MASK  		      0x000F0000
#define REG_2_MASK 		      0x0000F000
#define REG_3_MASK 		      0x00000F00
#define REG_4_MASK 		      0x0000000F	
#define OPERAND_MASK                  0x00000FFF 
#define OFFSET_1_MASK                 0x00000FFF 
#define OFFSET_2_MASK                 0x00FFFFFF

#define IMMEDIATE_VALUE_MASK	      0x000000FF
#define ROTATE_FIELD_MASK             0x00000F00
#define BIT_4_MASK 		      0x00000010
#define RM_REG_MASK 		      0x0000000F	
#define SHIFT_VALUE_MASK 	      0x00000F80
#define SHIFT_TYPE_MASK               0x00000060
#define RS_REG_MASK 		      0x00000F00
#define RS_VAL_MASK                   0x000000FF
#define RESULT_BIT_MASK               0x80000000

/* Global variables */

/*This variable will always either be a 0 or a 1, it will be set and checked
 * as appropriate throughout the program */
int carry_out_flag = 0;

/* Pointer definitions */
struct cpu *cpu_ptr;
instr_flags *instr_flags_ptr;
uint32_t pc = 0;                      /* Program counter will act as a pointer to memory*/


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
 * Extracts the relevant bits and sets them in the Mult struct
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
 * Extracts the relevant bits and sets them in the Single Data Trans struct
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
 * Extracts the relevant bits and sets them in the Branch struct
 * @param instr The instruction word
 */
void
decode_branch(uint32_t instr){
	//get offset
	uint32_t offset = extract_bits(instr, OFFSET_1_MASK, 0);
	instr_branch_ptr->offset = offset;
}
 

/* Execution of instructions */


/**
 * Executes Data Processing Instruction
 */
void
execute_data_proc(){

    uint32_t operand_2_val = result_set_I_flag(I_flag_set());



    uint32_t left_operand = instr_data_proc_ptr->rn_reg;
    uint32_t op_code = instr_data_proc_ptr->op_code;
    uint32_t result = opcode_dispatch(op_code, left_operand, operand_2_val);

    if(S_flag_set()){
		/* Update CPSR flags */

		/* Set C flag */
		C_flag_set(op_code);

		/* Set Z flag */
		if(result == 0x0){
		    instr_flags_ptr->flag_Z = 1;
		}

		/* Set N flag */
		uint32_t result_bit_31 = extract_bits(result, RESULT_BIT_MASK, 31);
		instr_flags_ptr->flag_N = result_bit_31;
	}

	uint32_t rd_reg = instr_data_proc_ptr->rd_reg;

	register_select_write_opcode(op_code, result, rd_reg);
}


/**
 * Return the result depending on the I flag
 * @param I_flag_set One or zero depending on whether flag is set
 */
uint32_t
result_set_I_flag(int I_flag_set){	
	
        uint32_t operand_2_val = 0;
      
        if(I_flag_set){
		/* Operand2 is an immediate constant */
		uint32_t immediate_value = extract_bits(instr_data_proc_ptr->operand_2, IMMEDIATE_VALUE_MASK, 0);
		uint32_t rotate_field    = extract_bits(instr_data_proc_ptr->operand_2, ROTATE_FIELD_MASK, 8);

		/* Double rotation amount */
		uint32_t rotate_amount = rotate_field << 1;
		
		/* Rotate right */
		uint32_t rotated_result = execute_rotate_right(rotate_amount, immediate_value);

		operand_2_val = rotated_result;
	}
	else{
		/* Operand2 is a shifted register */
		
		/* Read contents of register Rm */
		uint32_t reg_rm = extract_bits(instr_data_proc_ptr->operand_2, RM_REG_MASK, 0);
		uint32_t reg_val = register_select_read(reg_rm);

		/* Examine bit 4 of Operand2 */
		uint32_t bit_4 = extract_bits(instr_data_proc_ptr->operand_2, BIT_4_MASK, 4);

		/* Extract shift type */
		uint32_t shift_type  = extract_bits(instr_data_proc_ptr->operand_2, SHIFT_TYPE_MASK, 5);
		
		if(bit_4){
			/* If bit 4 == 1 then shift is specified by register */

                        /* Read contents of register Rs */
			uint32_t reg_rs = extract_bits(instr_data_proc_ptr->operand_2, RS_REG_MASK, 8);
			uint32_t reg_rs_val = register_select_read(reg_rs);

			/* Extract bottom byte of Rs register */
			uint32_t bottom_byte = extract_bits(reg_rs_val, RS_VAL_MASK, 0);

			operand_2_val = shift_type_dispatch(shift_type, bottom_byte, reg_val);

		}
		else{
			/* Else shift by a constant amount */

			/* Integer shift value */
			uint32_t shift_amount = extract_bits(instr_data_proc_ptr->operand_2, SHIFT_VALUE_MASK, 7);

			operand_2_val = shift_type_dispatch(shift_type, shift_amount, reg_val);
		}
	}
	return operand_2_val;
}


/**
 * Sets the C flag 
 * @param op_code The opcode depends on 
 *                how the C flag is set
 */
void
C_flag_set(uint32_t op_code){

	switch(op_code){
		case OC_AND : case OC_EOR : case OC_ORR : 
		case OC_TEQ : case OC_TST : case OC_MOV : 
			instr_flags_ptr->flag_C = carry_out_flag; break;
        
		case OC_ADD : 
		
			if(carry_out_flag){
				instr_flags_ptr->flag_C = 1;
			}
			else{
				instr_flags_ptr->flag_C = 0;
			}
			; break;

		case OC_SUB : case OC_RSB : case OC_CMP : 
			
			if(carry_out_flag){
				instr_flags_ptr->flag_C = 0;
			}
			else{
				instr_flags_ptr->flag_C = 1;
			}
			; break;
			
		default : 
			printf("c flag set");
	}
}


/**
 * Depending on the opcode, it will write 
 * to the destination register
 * @param op_code The instruction opcode
 * @param result  The result which has to written (or not)
 * @param rd_reg  The destination register
 */
void
register_select_write_opcode(uint32_t op_code, uint32_t result, uint32_t rd_reg){

	switch(op_code){
		case OC_AND : register_select_write(result, rd_reg); break;
		case OC_EOR : register_select_write(result, rd_reg); break;
		case OC_SUB : register_select_write(result, rd_reg); break;
		case OC_RSB : register_select_write(result, rd_reg); break;
		case OC_ADD : register_select_write(result, rd_reg); break;
		case OC_TST : /* Don't write */ 
		case OC_TEQ : /* Don't write */
		case OC_CMP : /* Don't write */
		case OC_ORR : register_select_write(result, rd_reg); break;
		case OC_MOV : register_select_write(result, rd_reg); break;
		default : 
			printf("Opcode reg write error");
	}
}


/**
 * Selects which shift operation should be executed
 * @param shift_type   The type of shift operation 
 * @param shift_amount The amount the value should be shifted by
 * @param reg_val      The value stored in the register
 * @return The result after the shift operation is carries out
 */
uint32_t
shift_type_dispatch(uint32_t shift_type, uint32_t shift_amount, uint32_t reg_val){

	switch(shift_type){
		case LSL : 
			return execute_shift_type(&execute_logical_shift_left, shift_amount, reg_val);
		case LSR : 
			return execute_shift_type(&execute_logical_shift_right, shift_amount, reg_val);
                case ASR : 
			return execute_shift_type(&execute_arithmetic_shift_right, shift_amount, reg_val);
		case ROR : 
			return execute_shift_type(&execute_rotate_right, shift_amount, reg_val);
		default  :
			printf("Given shift type not supported");
	}
	return EXIT_FAILURE;
}


/**
 * Executes the Logical Shift Left operation
 * @param shift_amount The amount the value will be shifted by
 * @param reg_val      The value stored in the register
 * @return The result of carrying out the operation
 */
/*Note, we could find the msb using log2 if we can work out how to cast*/
int
most_significant_bit(uint32_t test){
    int i, temp;
    temp = test;
    for(i = 31; i < 0; i--){
        if((temp & 0x80000000) == 0x80000000){
            return i;
        } else {
            temp <<= 1;
        }
    }
    return 0;
}


/*Note, I am returning 31 here even if it is not the least significant bit
 *as this function is for the purpose of checking for underflow.*/
int
least_significant_bit(uint32_t test){
    int i, temp;
    temp = test;
    for(i = 0; i < 31; i++){
        if((temp & 0x1) == 0){
            temp >>= 1;
        } else {
            return i;
        }
    return 31;
    }
	return EXIT_FAILURE;
}


/**
 * Returns the result of executing this operaion
 * @param shift_amount The amount value will be shifted by
 * @param reg_val      The value stored in register to be manipulated
 * @return             The result of this operation 
 */
uint32_t
execute_logical_shift_left(uint32_t shift_amount, uint32_t reg_val){
    if(S_flag_set()){
        int msb = most_significant_bit(reg_val);
        if(msb + shift_amount > 31){
            carry_out_flag = 1;
        }
    }
        return (reg_val << shift_amount);
}


/* This function takes 2 parameters and sets the overflow flag if an overflow
 * occurs.
 * @param shift_amount is the number of places that reg_val should be shifted
 * @param reg_val is the value upon which the shift is to occur. */
void shift_right_flag_check(uint32_t shift_amount, uint32_t reg_val){
    if(S_flag_set()){
        int lsb = least_significant_bit(reg_val);
        if(lsb - shift_amount < 0){
            carry_out_flag = 1;   
        }
    }
} 


/**
 * Executes the Logical Shift Right operation
 * @param shift_amount The amount the value will be shifted by
 * @param reg_val      The value stored in the register
 * @return The result of carrying out the operation
 */
uint32_t
execute_logical_shift_right(uint32_t shift_amount, uint32_t reg_val){
    shift_right_flag_check(shift_amount, reg_val); 
    return (reg_val >> shift_amount);
}


/**
 * Executes the Arithmetic Shift Right operation
 * @param shift_amount The amount the value will be shifted by
 * @param reg_val      The value stored in the register
 * @return The result of carrying out the operation
 */
uint32_t
execute_arithmetic_shift_right(uint32_t shift_amount, uint32_t reg_val){
    int i, return_val, bit31;
    shift_right_flag_check(shift_amount, reg_val);
    bit31 = reg_val & 0x80000000;
    return_val = reg_val;
    for(i = 0; i < shift_amount; i++){
        return_val >>= 1;
        return_val += bit31; /*possibly do this with bitwise or? */
    }
    return return_val;
}


/**
 * Executes the Rotate Right operation
 * @param shift_amount The amount the value will be shifted by
 * @param reg_val      The value stored in the register
 * @return The result of carrying out the operation
 */
uint32_t
execute_rotate_right(uint32_t shift_amount, uint32_t reg_val){
    shift_right_flag_check(shift_amount, reg_val);
    return((reg_val >> shift_amount) | (reg_val << (32 - shift_amount))); /*possible bug*/
    
}


/**
 * Passes the given function pointer and parameters to 
 * the selected shift operation
 * @param (*execute_shift_type_ptr)(uint32_t, uint32_t) 
 *     Shift types function pointer
 * @param shift_amount The amount the value will be shifted by
 * @param reg_val      The value stored in the register
 * @return The result of carrying out the operation to the top level
 */
uint32_t
execute_shift_type(uint32_t (*execute_shift_type_ptr)(uint32_t, uint32_t),
		uint32_t shift_amount, uint32_t reg_val){
	return (*execute_shift_type_ptr)(shift_amount, reg_val);
}


/**
 * Selects which opcode should be carried out
 * @param opcode        The given opcode
 * @param left_operand  Used to carry out opcode
 * @param right_operand Used to carry out opcode
 */
uint32_t
opcode_dispatch(uint32_t opcode, uint32_t left_operand, uint32_t right_operand){

	switch(opcode){
		case OC_AND : 
			return execute_op_code(&execute_op_code_add, left_operand, right_operand);	
		case OC_EOR :
			return execute_op_code(&execute_op_code_eor, left_operand, right_operand);
		case OC_SUB :
			return execute_op_code(&execute_op_code_sub, left_operand, right_operand);
		case OC_RSB :
			return execute_op_code(&execute_op_code_rsb, left_operand, right_operand);
		case OC_ADD :
			return execute_op_code(&execute_op_code_add, left_operand, right_operand);
		case OC_TST :
			return execute_op_code(&execute_op_code_tst, left_operand, right_operand);
		case OC_TEQ :
			return execute_op_code(&execute_op_code_teq, left_operand, right_operand);
		case OC_CMP :
			return execute_op_code(&execute_op_code_cmp, left_operand, right_operand);
		case OC_ORR :
			return execute_op_code(&execute_op_code_orr, left_operand, right_operand);
		case OC_MOV :
			return execute_op_code(&execute_op_code_mov, left_operand, right_operand);
		default :
			printf("Unsupported opcode selected");
	}
	return EXIT_FAILURE;
}


/**
 * Executes the AND operation 
 * @param reg The value stored in the register
 * @param operand2 The value after doing rotate/shift operation
 */
uint32_t
execute_op_code_and(uint32_t reg, uint32_t operand2){
	return reg & operand2;
}


/**
 * Executes the EOR operation 
 * @param reg The value stored in the register
 * @param operand2 The value after doing rotate/shift operation
 */
uint32_t
execute_op_code_eor(uint32_t reg, uint32_t operand2){
	return reg ^ operand2; 
}


/**
 * This is simply a bitwise add function, the algorithm is from wikipedia
 * it is in a helper method as the checks for carry flags are different in
 * subtract and add, possible to improve efficiency by checking which is larger
 * but I don't feel that this is necessary
 * @param reg is a uint32_t which is added
 * @param operand2 is a uint32_t which is added
 */
uint32_t
bitwise_add(uint32_t reg, uint32_t operand2){
    uint32_t x, y, sum, carry;
    sum = reg ^ operand2;
    carry = reg & operand2;
    while(carry != 0){
        carry <<= 1;
        x = sum;
        y = carry;
        sum = x ^ y;
        carry = x & y;
    }
    return sum;
}


/**
 * Executes the SUB operation 
 * **Not sure if this is correct**
 * @param reg The value stored in the register
 * @param operand2 The value after doing rotate/shift operation
 */
uint32_t
execute_op_code_sub(uint32_t reg, uint32_t operand2){
    if(S_flag_set()){
        int i;
        for(i = 0; i < 31; i++){
             if(most_significant_bit(reg) < 
                most_significant_bit(operand2)){
                carry_out_flag = 1; break;
             } else if(most_significant_bit(reg) > 
                most_significant_bit(operand2)){
                break;
             }
         }
/*the loop reaching the end means that they are equal*/
         return 0;
         }
	return EXIT_FAILURE;
}


/**
 * Executes the RSB operation 
 * @param reg The value stored in the register
 * @param operand2 The value after doing rotate/shift operation
 */
uint32_t
execute_op_code_rsb(uint32_t reg, uint32_t operand2){
    return execute_op_code_sub(operand2, reg);
}


/**
 * Executes the ADD operation
 * Algorithm from wikipedia pageon bitwise operations 
 * @param reg The value stored in the register
 * @param operand2 The value after doing rotate/shift operation
 */
uint32_t
execute_op_code_add(uint32_t reg, uint32_t operand2){
    if(S_flag_set()){
        if((most_significant_bit(reg) == 31 
          && (most_significant_bit(operand2)) == 31)){
            carry_out_flag = 1;
        }
    }
    return bitwise_add(reg, operand2);
}


/**
 * Executes the TST operation 
 * @param reg The value stored in the register
 * @param operand2 The value after doing rotate/shift operation
 */
uint32_t
execute_op_code_tst(uint32_t reg, uint32_t operand2){
    return reg & operand2;
}


/**
 * Executes the TEQ operation 
 * @param reg The value stored in the register
 * @param operand2 The value after doing rotate/shift operation
 */
uint32_t
execute_op_code_teq(uint32_t reg, uint32_t operand2){
    return reg ^ operand2;
}


/**
 ***I have assumed that the lack of saving is done at a higher level and so** 
  ** have just called sub here THE ALTERNATIVE is that we set the bit flag 
     and do not need to calculate the result, in which case move the 
     "carry_out_flag check" section of sub to a helper method and just call this
 * Executes the CMP operation 
 * @param reg The value stored in the register
 * @param operand2 The value after doing rotate/shift operation
 */
uint32_t
execute_op_code_cmp(uint32_t reg, uint32_t operand2){
    return execute_op_code_sub(reg, operand2);	
}


/**
 * Executes the ORR operation 
 * @param reg The value stored in the register
 * @param operand2 The value after doing rotate/shift operation
 */
uint32_t
execute_op_code_orr(uint32_t reg, uint32_t operand2){
    return reg | operand2;
}


/**
 * Executes the MOV operation 
 * @param reg The value stored in the register
 * @param operand2 The value after doing rotate/shift operation
 */
uint32_t
execute_op_code_mov(uint32_t reg, uint32_t operand2){
    /* Not sure if this is correct */
    return operand2;
}


/**
 * Passes the given function pointer and parameters to 
 * the selected opcode
 * @param  uint32_t (*execute_op_code_ptr)(uint32_t, uint32_t)
 *     opcode function pointer
 * @param left_operand  An value used when carrying out the opcode
 * @param right_operand An value used when carrying out the opcode
 * @return The result of carrying out the operation to the top level
 */
uint32_t
execute_op_code(uint32_t (*execute_op_code_ptr)(uint32_t, uint32_t), 
		uint32_t left_operand, uint32_t right_operand){
	return (*execute_op_code_ptr)(left_operand, right_operand);
}


/**
 * Checks whether I flag is set
 */
int
I_flag_set(){
	if(instr_data_proc_ptr->I_flag == 1){
		return EXIT_SUCCESS;
	}
	else{
		return EXIT_FAILURE;
	}
}


/**
 * Carries out the Multiply instruction
 */
void
execute_mult(){

	if(A_flag_set()){
		/* Perform a multiply and accumulate */
		multiply_rm_rs();
    	accumulate_rm_rs_rn();
	}
	else{
		/* Perform only multiply */
		multiply_rm_rs();
	}

	/* If S bit is set, update N and Z flags of CPSR*/
	if(S_flag_set()){
		uint32_t rd_reg = instr_mult_ptr->rd_reg;
		uint32_t result = register_select_read(rd_reg);

		/* Set Z flag iff result is 0 */
		if(result == 0){
			instr_flags_ptr->flag_Z = 1;
		}

		/* Set N flag to bit 31 of result */
		uint32_t flag_N_result = extract_bits(result, 0x40000000, 30);
		instr_flags_ptr->flag_N = flag_N_result;
	}
}


/**
 * Multiplies the contents of two registers
 */
void 
multiply_rm_rs(){

	/* TODO : check which value is larger */
	/* TODO : the multiply is bit wise not int */
	uint32_t rm_reg = instr_mult_ptr->rm_reg;
	uint32_t rs_reg = instr_mult_ptr->rs_reg;
	uint32_t rd_reg = instr_mult_ptr->rd_reg;

	uint32_t rm_reg_contents = register_select_read(rm_reg);
	uint32_t rs_reg_contents = register_select_read(rs_reg);
 
	uint32_t result_mult = rm_reg_contents * rs_reg_contents;

	register_select_write(result_mult, rd_reg);
}


/**
 * Adds the contents of a register the the destination register
 */
void
accumulate_rm_rs_rn(){
	
	uint32_t rn_reg = instr_mult_ptr->rn_reg;
	uint32_t rd_reg = instr_mult_ptr->rd_reg;

	uint32_t rn_reg_contents = register_select_read(rn_reg);
	uint32_t rd_reg_contents = register_select_read(rd_reg);

	uint32_t result = rn_reg_contents + rd_reg_contents;

	register_select_write(result, rd_reg);
}


/**
 * Writes the result of calculation to the correct register
 */
void
register_select_write(uint32_t calc, uint32_t reg){

	switch(reg){
		case R0 : cpu_ptr->r0 = calc; break;
		case R1 : cpu_ptr->r1 = calc; break;
		case R2 : cpu_ptr->r2 = calc; break;
		case R3 : cpu_ptr->r3 = calc; break;
		case R4 : cpu_ptr->r4 = calc; break;
		case R5 : cpu_ptr->r5 = calc; break;
		case R6 : cpu_ptr->r6 = calc; break;
		case R7 : cpu_ptr->r7 = calc; break;
		case R8 : cpu_ptr->r8 = calc; break;
		case R9 : cpu_ptr->r9 = calc; break;
		case R10 : cpu_ptr->r10 = calc; break;
		case R11 : cpu_ptr->r11 = calc; break;
		case R12 : cpu_ptr->r12 = calc; break;
		default :printf("Invalid reg");
	}
}


/**
 * Reads the content of the selected register
 */
uint32_t 
register_select_read(uint32_t reg){

	switch(reg){
		case R0 : return cpu_ptr->r0; 
		case R1 : return cpu_ptr->r1; 
		case R2 : return cpu_ptr->r2; 
		case R3 : return cpu_ptr->r3; 
		case R4 : return cpu_ptr->r4; 
		case R5 : return cpu_ptr->r5; 
		case R6 : return cpu_ptr->r6; 
		case R7 : return cpu_ptr->r7; 
		case R8 : return cpu_ptr->r8; 
		case R9 : return cpu_ptr->r9; 
		case R10 : return cpu_ptr->r10; 
		case R11 : return cpu_ptr->r11; 
		case R12 : return cpu_ptr->r12; 
		default :printf("Invalid reg");
	}
	return EXIT_FAILURE;
}


/**
 * Checks if the A flag is set
 */
int
A_flag_set(void){
	if(instr_mult_ptr->A_flag == 1){
		return EXIT_SUCCESS;
	}
	else{
		return EXIT_FAILURE;
	}
}


/**
 * Checks if the S flag is set
 */
int
S_flag_set(void){
	if(instr_mult_ptr->S_flag == 1){
		return EXIT_SUCCESS;
	}
	else{
		return EXIT_FAILURE;
	}
}


/**
 * Carries out the Single Data Transfer Instruction 
 */
void
execute_single_data_trans(){

	/* The opposite is true for data processing instructions*/
	uint32_t offset_value = result_set_I_flag(!I_flag_set());


	uint32_t base_reg = instr_single_data_trans_ptr->rn_reg;
    uint32_t base_reg_contents = register_select_read(base_reg);
	
	uint32_t s_or_d_reg = instr_single_data_trans_ptr->rd_reg;
	uint32_t s_or_d_reg_contents = register_select_read(s_or_d_reg);
	
	
	uint32_t memory_access_index = 0;

	if(P_flag_set()){
		/* Pre-Indexing*/
		/* Offset is added/subtracted to the 
		 * base register before transferring the data.*/

        if(U_flag_set()){
			/* Offset is added to base register */
			memory_access_index = base_reg_contents + offset_value;
		}
		else{
			/* Offset is subtracted from base register */
			memory_access_index = base_reg_contents - offset_value;
		}

        /* Transfer data */

		if(L_flag_set()){
			/* Load word from memory */
			uint32_t word_load = memory_fetch_word(memory_access_index);
			register_select_write(word_load, s_or_d_reg);
		}
		else{
			/* Store word in memory */
			uint32_t word_store = s_or_d_reg_contents;
			memory_write_word(memory_access_index, word_store);
		}

	}
	else{
		/* Post-Indexing*/
		/* Offset is added/subtracted to the 
		 * base register after transferring the data.*/
        
		/* Transfer data */

		if(L_flag_set()){
			/* Load word from memory */
			uint32_t word_load = memory_fetch_word(memory_access_index);
			register_select_write(word_load, s_or_d_reg);
		}
		else{
			/* Store word in memory */
			uint32_t word_store = s_or_d_reg_contents;
			memory_write_word(memory_access_index, word_store);
		}



		if(U_flag_set()){
			/* Offset is added to base register */
			memory_access_index = base_reg_contents + offset_value;
		}
		else{
			/* Offset is subtracted from base register */
			memory_access_index = base_reg_contents - offset_value;
		}
		
		instr_single_data_trans_ptr->rn_reg += offset_value; 
	}
}


/**
 * Checks if the L falg is set
 */
int
L_flag_set(){
	if(instr_single_data_trans_ptr->L_flag == 1){
		return EXIT_SUCCESS;
	}
	else{
		return EXIT_FAILURE;
	}
}


/**
 * Checks if the P falg is set
 */
int
P_flag_set(){
	if(instr_single_data_trans_ptr->P_flag == 1){
		return EXIT_SUCCESS;
	}
	else{
		return EXIT_FAILURE;
	}
}


/**
 * Checks if the U falg is set
 */
int
U_flag_set(){
	if(instr_single_data_trans_ptr->U_flag == 1){
		return EXIT_SUCCESS;
	}
	else{
		return EXIT_FAILURE;
	}
}


/**
 * Carries out the Branch Instruction
 */
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
cpu_cycle(void){

	/* Before cpu struct pointer is passed in we need to initialise it */

	uint32_t instr = 0;
        pc = cpu_ptr->pc;
	/* Fetch one instruction from memory */
	instr = memory_fetch_word(pc);
	cpu_ptr->pc = pc + 4;

	/* Check if all-0 instruction first */

	if(instr == 0){
		/* Upon termination output the state of all the registers */
	}
	
	instr_decode(instr);
	instr_execute(instr);
}


/**
 * Prints the values of each of the registers
 * (Yes, I cried as I wrote this)
 * No parameters (may need the cpu parameter)
 */
void
print_registers(){
    printf("%s", "The register r0 contains: ");
    printf("%x", cpu_ptr->r0);
   
    printf("%s", "The register r1 contains: ");
    printf("%x", cpu_ptr->r1);
   
    printf("%s", "The register r2 contains: ");
    printf("%x", cpu_ptr->r2);  
  
    printf("%s", "The register r3 contains: ");
    printf("%x", cpu_ptr->r3);  
  
    printf("%s", "The register r4 contains: ");
    printf("%x", cpu_ptr->r4);

    printf("%s", "The register r5 contains: ");
    printf("%x", cpu_ptr->r5);

    printf("%s", "The register r6 contains: ");
    printf("%x", cpu_ptr->r6);

    printf("%s", "The register r7 contains: ");
    printf("%x", cpu_ptr->r7);

    printf("%s", "The register r8 contains: ");
    printf("%x", cpu_ptr->r8);

    printf("%s", "The register r9 contains: ");
    printf("%x", cpu_ptr->r9);

    printf("%s", "The register r10 contains: ");
    printf("%x", cpu_ptr->r10);

    printf("%s", "The register r11 contains: ");
    printf("%x", cpu_ptr->r11);

    printf("%s", "The register r12 contains: ");
    printf("%x", cpu_ptr->r12);

    printf("%s", "The register rsp contains: ");
    printf("%x", cpu_ptr->rsp);

    printf("%s", "The register lr contains: ");
    printf("%x", cpu_ptr->lr);

    printf("%s", "The register pc contains: ");
    printf("%x", cpu_ptr->pc);

    printf("%s", "The register cpsr contains: ");
    printf("%x", cpu_ptr->cpsr);
}
