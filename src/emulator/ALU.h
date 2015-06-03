#ifndef ALU_H
#define ALU_H


#include "common_instances.h"


int I_flag_set_data_proc(void); 
int A_flag_set(void);
int S_flag_set_data_proc(void);
int S_flag_set_mult(void);
int I_flag_set_single_data_trans(void);
int L_flag_set(void);
int P_flag_set(void);
int U_flag_set(void);
void C_flag_set(uint32_t op_code);
void update_CPSR();


void shift_right_flag_check(uint32_t shift_amount, uint32_t reg_val);
int most_significant_bit(uint32_t test);
int least_significant_bit(uint32_t test);
uint32_t execute_logical_shift_left(uint32_t shift_amount, uint32_t reg_val);
uint32_t execute_logical_shift_right(uint32_t shift_amount, uint32_t reg_val);
uint32_t execute_arithmetic_shift_right(uint32_t shift_amount, uint32_t reg_val);
uint32_t execute_rotate_right(uint32_t shift_amount, uint32_t reg_val);
uint32_t execute_op_code_and(uint32_t reg, uint32_t operand2);
uint32_t execute_op_code_eor(uint32_t reg, uint32_t operand2);
uint32_t execute_op_code_sub(uint32_t reg, uint32_t operand2);
uint32_t execute_op_code_rsb(uint32_t reg, uint32_t operand2);
uint32_t execute_op_code_add(uint32_t reg, uint32_t operand2);
uint32_t execute_op_code_tst(uint32_t reg, uint32_t operand2);
uint32_t execute_op_code_teq(uint32_t reg, uint32_t operand2);
uint32_t execute_op_code_cmp(uint32_t reg, uint32_t operand2);
uint32_t execute_op_code_orr(uint32_t reg, uint32_t operand2);
uint32_t execute_op_code_mov(uint32_t reg, uint32_t operand2);
void multiply_rm_rs();
void accumulate_rm_rs_rn();


#endif
