#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "common_instances.h"

#define INSTR_DATA_PROC_HANDLER(X) fun_name(X)

/* ARM instruction set structs */

typedef struct{
	arm_cc cond_code;

	//instr_flags->flag_I; 

	arm_oc op_code;

	// opcodeselect;

	instr_flags *flags;

	cpu_reg reg_n;

	cpu_reg reg_d;

	[TYPE????   operand2]	


}instr_data_proc;

op_code = OC_AND;
op_code = 0x0;

INSTR_DATA_PROC_HANDLER(inst)


int (*name(int shit))(int) {

}

typedef struct{
}instr_mult;

typedef struct{
}instr_single_data_transfer;

typedef struct{
}instr_branch;

#endif
