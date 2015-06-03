#include "common_instances.h"


/* Global variables */


/**
 * This variable will always either be 0 or 1. It will be
 * set and checked as appropriate throughout the program 
 */
int carry_out_flag = 0;


/* Flag Operations */


/**
 * Checks whether I flag is set for Data Processing
 */
int
I_flag_set_data_proc(void){
  return instr_data_proc_ptr->I_flag;
}


/**
 * Checks if the A flag is set
 */
int
A_flag_set(void){
  return instr_mult_ptr->A_flag;
}


/**
 * Checks if the S flag is set in Data Processing
 */
int
S_flag_set_data_proc(void){
  return instr_data_proc_ptr->S_flag;
}


/**
 * Checks if the S flag is set in Multiply
 */
int
S_flag_set_mult(void){
  return instr_mult_ptr->S_flag;
}


/**
 * Checks whether I flag is set for Single Data Transfer
 */
int
I_flag_set_single_data_trans(void){
  return instr_single_data_trans_ptr->I_flag;
}


/**
 * Checks if the L falg is set
 */
int
L_flag_set(void){
  return instr_single_data_trans_ptr->L_flag;
}


/**
 * Checks if the P flag is set
 */
int
P_flag_set(void){
  return instr_single_data_trans_ptr->P_flag;
}


/**
 * Checks if the U flag is set
 */
int
U_flag_set(void){
  return instr_single_data_trans_ptr->U_flag;
}


/**
 * Sets the C flag 
 * @param op_code The opcode indicates how the C flag should be set
 */
void
C_flag_set(uint32_t op_code){

  switch(op_code){
    case OC_AND : case OC_EOR : case OC_ORR : 
    case OC_TEQ : case OC_TST : case OC_MOV : 
      instr_flags_ptr->flag_C = carry_out_flag; 
      break;
    case OC_ADD : 
      if(carry_out_flag){
        instr_flags_ptr->flag_C = 1;
      }
      else{
        instr_flags_ptr->flag_C = 0;
      };
      break;
    case OC_SUB : case OC_RSB : case OC_CMP : 
    default : 
      break;
	}
}


/**
 * Updates the CPSR register
 */
void
update_CPSR(void){
  //Concatenate all four flags together
  uint32_t cpsr_flag = (instr_flags_ptr->flag_N) << 3 | (instr_flags_ptr->flag_Z) << 2 |
                       (instr_flags_ptr->flag_C) << 1 | (instr_flags_ptr->flag_V);

  //Shift left bottom four bits by 28 bits
  uint32_t cpsr_val = cpsr_flag << 28;

  //Assign the flag value
  cpu_ptr->cpsr = cpsr_val;
}


/* Sets the overflow flag if an overflow occurs.
 * @param shift_amount is the number of places that reg_val should be shifted
 * @param reg_val is the value upon which the shift is to occur. 
 */
void 
shift_right_flag_check(uint32_t shift_amount, uint32_t reg_val){
	
  if(S_flag_set_data_proc()){
    int lsb = least_significant_bit(reg_val);
    if((int32_t)lsb - (int32_t)shift_amount < 0){
      carry_out_flag = 1;   
    }
  }
} 


/* ALU Operations */


/**
 * Extracts the most significant bit 
 * @param test The value to be examined
 */
int
most_significant_bit(uint32_t test){
  int i, temp;
  temp = test;
  for(i = 31; i < 0; i--){
    if((temp & 0x80000000) == 0x80000000){
      return i;
    }
    else{
      temp <<= 1;
    }
  }
  return 0;
}


/**
 * Checks for underflow by examining the least significant bit
 * @param test The value to be examined
 * @return Returns 31 even if it is not the least significant bit
 */
int
least_significant_bit(uint32_t test){
  int i, temp;
  temp = test;
  for(i = 0; i < 31; i++){
    if((temp & 0x1) == 0){
    temp >>= 1;
    }
    else{
      return i;
    }
    return 31;
  }
  return 0;
}


/**
 * Executes Logical Shift Left
 * @param shift_amount The amount value will be shifted by
 * @param reg_val      The value stored in register to be manipulated
 */
uint32_t
execute_logical_shift_left(uint32_t shift_amount, uint32_t reg_val){
  
  if(S_flag_set_data_proc()){
    int msb = most_significant_bit(reg_val);
    if(msb + shift_amount > 31){
      carry_out_flag = 1;
    }
  }
  return (reg_val << shift_amount);
}


/**
 * Executes Logical Shift Right
 * @param shift_amount The amount the value will be shifted by
 * @param reg_val      The value stored in the register
 */
uint32_t
execute_logical_shift_right(uint32_t shift_amount, uint32_t reg_val){
  shift_right_flag_check(shift_amount, reg_val); 
  return (reg_val >> shift_amount);
}


/**
 * Executes Arithmetic Shift Right
 * @param shift_amount The amount the value will be shifted by
 * @param reg_val      The value stored in the register
 */
uint32_t
execute_arithmetic_shift_right(uint32_t shift_amount, uint32_t reg_val){
   
  uint32_t i, return_val, bit31;
  shift_right_flag_check(shift_amount, reg_val);
  bit31 = reg_val & 0x80000000;
  return_val = reg_val;
  for(i = 0; i < shift_amount; i++){
    return_val >>= 1;
    return_val += bit31; 
  }
  return return_val;
}


/**
 * Executes Rotate Right 
 * @param shift_amount The amount the value will be shifted by
 * @param reg_val      The value stored in the register
 */
uint32_t
execute_rotate_right(uint32_t shift_amount, uint32_t reg_val){
  shift_right_flag_check(shift_amount, reg_val);
  return((reg_val >> shift_amount) | (reg_val << (32 - shift_amount)));    
}


/**
 * Executes AND  
 * @param reg_contents The value stored in the register
 * @param value        The result after doing rotate/shift operations
 */
uint32_t
execute_op_code_and(uint32_t reg_contents, uint32_t value){
	return reg_contents & value;
}


/**
 * Executes EOR  
 * @param reg_contents The value stored in the register
 * @param value        The result after doing rotate/shift operations
 */
uint32_t
execute_op_code_eor(uint32_t reg_contents, uint32_t value){
	return reg_contents ^ value; 
}


/**
 * Executes SUB  
 * @param reg_contents The value stored in the register
 * @param value        The result after doing rotate/shift operation
 */
uint32_t
execute_op_code_sub(uint32_t reg_contents, uint32_t operand2){

  if (reg_contents == operand2){
    if (S_flag_set_data_proc()){
      instr_flags_ptr->flag_Z = 1;
      instr_flags_ptr->flag_C = 1;
    }
    return (uint32_t) 0;
  }
  
  int32_t op2compl = ~operand2 + 1; //Add 1 to get negative value because of 2's complement
  int32_t result = reg_contents + op2compl;

  if (reg_contents > operand2){
    if (S_flag_set_data_proc()){
      instr_flags_ptr->flag_C = 1;
      instr_flags_ptr->flag_Z = 0;
    }
    return result;
  }
  else{
    if(S_flag_set_data_proc()){
      instr_flags_ptr->flag_Z = reg_contents == operand2? 1: 0;
      instr_flags_ptr->flag_C = 0;

      //Get LSB as we are in Little Endian
      uint32_t result_bit_31 = extract_bits(result, 1, 0);
      instr_flags_ptr->flag_N = result_bit_31;
    }
    return result;
  }
}


/**
 * Executes RSB  
 * @param reg_contents The value stored in the register
 * @param value        The result after doing rotate/shift operations
 */
uint32_t
execute_op_code_rsb(uint32_t reg_contents, uint32_t value){
  return execute_op_code_sub(value, reg_contents);
}


/**
 * Executes ADD
 * @param reg_contents The value stored in the register
 * @param value        The result after doing rotate/shift operations
 */
uint32_t
execute_op_code_add(uint32_t reg_contents, uint32_t value){
  uint32_t result = reg_contents + value;

  //If addition exceeds maximum of 32-bit val, then result wraps around
  if (S_flag_set_data_proc()){
    if (result < reg_contents || result < value){
      carry_out_flag = 1;
    }
    else{
      carry_out_flag = 0;
    }
  }
  return result;
}


/**
 * Executes TST 
 * @param reg_contents The value stored in the register
 * @param value        The result after doing rotate/shift operations
 */
uint32_t
execute_op_code_tst(uint32_t reg_contents, uint32_t value){
  return reg_contents & value;
}


/**
 * Executes TEQ  
 * @param reg_contents The value stored in the register
 * @param value        The result after doing rotate/shift operations
 */
uint32_t
execute_op_code_teq(uint32_t reg_contents, uint32_t value){
  return reg_contents ^ value;
}


/**
 * Executes CMP  
 * @param reg_contents The value stored in the register
 * @param value        The result after doing rotate/shift operations
 */
uint32_t
execute_op_code_cmp(uint32_t reg_contents, uint32_t value){
  return execute_op_code_sub(reg_contents, value);	
}


/**
 * Executes ORR
 * @param reg_contents The value stored in the register
 * @param value        The result after doing rotate/shift operations
 */
uint32_t
execute_op_code_orr(uint32_t reg_contents, uint32_t value){
  return reg_contents | value;
}


/**
 * Executes MOV 
 * @param reg_contents The value stored in the register
 * @param value        The result after doing rotate/shift operations
 */
uint32_t
execute_op_code_mov(uint32_t reg_contents, uint32_t value){
  return value;
}


/**
 * Multiplies the contents of two registers
 */
void
multiply_rm_rs(void){

  //Extract relevant instruction fields
  uint32_t rm_reg = instr_mult_ptr->rm_reg;
  uint32_t rs_reg = instr_mult_ptr->rs_reg;
  uint32_t rd_reg = instr_mult_ptr->rd_reg;

  uint32_t rm_reg_contents = register_select_read(rm_reg);
  uint32_t rs_reg_contents = register_select_read(rs_reg);
 
  uint32_t result_mult = rm_reg_contents * rs_reg_contents; 

  register_select_write(result_mult, rd_reg);
}


/**
 * Adds the contents of a register to the destination register
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

