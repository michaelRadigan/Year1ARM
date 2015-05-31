#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "common_instances.h"


/* ARM instruction set structs */

/* Try to use enums somehow in structs/functions */

typedef struct{

	uint32_t cond;
	uint32_t I_flag;
	uint32_t op_code;
	uint32_t S_flag;
	uint32_t rn_reg;
	uint32_t rd_reg;
	uint32_t operand_2;
	
}instr_data_proc_struct;

extern instr_data_proc_struct *instr_data_proc_ptr;


typedef struct{

	uint32_t cond;
	uint32_t A_flag;
	uint32_t S_flag;
	uint32_t rd_reg;
	uint32_t rn_reg;
	uint32_t rs_reg;
	uint32_t rm_reg;

}instr_mult_struct;

extern instr_mult_struct *instr_mult_ptr;


typedef struct{

	uint32_t cond;
	uint32_t I_flag;
	uint32_t P_flag;
	uint32_t U_flag;
	uint32_t L_flag;
	uint32_t rn_reg;
	uint32_t rd_reg;
	uint32_t offset;

}instr_single_data_trans_struct;

extern instr_single_data_trans_struct *instr_single_data_trans_ptr;


typedef struct{

	uint32_t cond;
	uint32_t offset;

}instr_branch_struct;

extern instr_branch_struct *instr_branch_ptr;

#endif
