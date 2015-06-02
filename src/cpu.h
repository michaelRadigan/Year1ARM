#ifndef CPU_H
#define CPU_H

#include "common_instances.h"


typedef struct cpu{
	uint32_t r0;
	uint32_t r1;
	uint32_t r2;
	uint32_t r3;
	uint32_t r4;
	uint32_t r5;
	uint32_t r6;
	uint32_t r7;
	uint32_t r8;
	uint32_t r9;
	uint32_t r10;
	uint32_t r11;
	uint32_t r12;
	uint32_t rsp;// Is ignored in this implementation
	uint32_t rlr;// Is ignored in this implementation
	uint32_t pc;
	uint32_t cpsr; 
}cpu;

extern cpu *cpu_ptr;


/**
 * Flags
 */
typedef struct{
	/* All flags are 1 bit wide */ 
	
	/* Instruction Flags */
	int flag_I;
	int flag_S;
	int flag_A;
	int flag_P;
	int flag_U;
	int flag_L;

	/* CSPR register flags */
	int flag_N;//The last result was negative
	int flag_Z;//The last result was zero
	int flag_C;//The last result caused a bit to be carried out
	int flag_V;//The last result overflowed

	/* Carry flag used in arithmetic and shift operation */
	int flag_carry;

}instr_flags;

extern instr_flags *instr_flags_ptr;


/**
 * ARM Condition Codes
 */
typedef enum{
	CC_EQ = 0x0,
	CC_NE = 0x1,
	CC_GE = 0xA,
	CC_LT = 0xB,
	CC_GT = 0xC,
	CC_LE = 0xD,
	CC_AL = 0xE
}arm_cc;


/**
 * ARM Operation Codes
 */
typedef enum{
	/* Continuity of OpCodes is not
	 * maintained because we are only
	 * dealing with a subset of them
	 */
	OC_AND = 0x0,
	OC_EOR = 0x1,
	OC_SUB = 0x2,
	OC_RSB = 0x3,
	OC_ADD = 0x4,
	OC_TST = 0x8,
	OC_TEQ = 0x9,
	OC_CMP = 0xA,
	OC_ORR = 0xC,
	OC_MOV = 0xD
}arm_oc;


/**
 * ARM Shift Types
 */
typedef enum{
	LSL = 0x0,
	LSR = 0x1,
	ASR = 0x2,
	ROR = 0x3
}arm_shift;


/**
 * CPU registers
 */
typedef enum{
	R0   = 0x0,
	R1   = 0x1,
	R2   = 0x2,
	R3   = 0x3,
	R4   = 0x4,
	R5   = 0x5,
	R6   = 0x6,
	R7   = 0x7,
	R8   = 0x8,
	R9   = 0x9,
	R10  = 0xA,
	R11  = 0xB,
	R12  = 0xC,
	RSP  = 0xD,// Is ignored in this implementation
	RLR  = 0xE,// Is ignored in this implementation
	PC   = 0xF,
	CPSR = 0x10
}cpu_reg;

//TODO dont use so can elete
#define PIPELINE_SIZE 2


typedef struct {
	uint32_t fetched;
	uint32_t decoded;
}pipeline;

extern pipeline *pipeline_ptr;


int pipeline_push(uint32_t pc);
uint32_t pipeline_pop(void);


/* Function prototypes */

int check_bits(uint32_t instr, uint32_t mask, int shift, uint32_t expected);
uint32_t extract_bits(uint32_t instr, uint32_t mask, uint32_t shift);

int instr_data_proc(uint32_t instr);
int instr_mult(uint32_t instr);
int instr_single_data_trans(uint32_t instr);
int instr_branch(uint32_t instr);

int check_instr_cond_code(uint32_t instr);

void decode_data_proc(uint32_t instr);
void decode_mult(uint32_t instr);
void decode_single_data_trans(uint32_t instr);
void decode_branch(uint32_t instr);



int I_flag_set_data_proc(); 
int A_flag_set(void);
int S_flag_set_data_proc(void);
int S_flag_set_mult(void);
int I_flag_set_single_data_trans();

int L_flag_set(void);
int P_flag_set(void);
int U_flag_set(void);

void C_flag_set(uint32_t op_code);

void update_CPSR();




void execute_data_proc();


uint32_t result_set_I_flag(int I_flag_set, uint32_t operand_2_or_offset);


void register_select_write_opcode(uint32_t opcode, uint32_t result, uint32_t rd_reg);
uint32_t shift_type_dispatch(uint32_t shift_type, uint32_t shift_amount, uint32_t reg_val);
int most_significant_bit(uint32_t test);
int least_significant_bit(uint32_t test);
uint32_t execute_logical_shift_left(uint32_t shift_amount, uint32_t reg_val);
uint32_t execute_logical_shift_right(uint32_t shift_amount, uint32_t reg_val);
uint32_t execute_arithmetic_shift_right(uint32_t shift_amount, uint32_t reg_val);
uint32_t execute_rotate_right(uint32_t shift_amount, uint32_t reg_val);
uint32_t execute_shift_type(uint32_t (*execute_shift_type_ptr)(uint32_t, uint32_t),
		uint32_t shift_amount, uint32_t reg_val);
uint32_t opcode_dispatch(uint32_t opcode, uint32_t left_operand, uint32_t right_operand);
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

uint32_t execute_op_code(uint32_t (*execute_op_code_ptr)(uint32_t, uint32_t),
	    uint32_t left_operand, uint32_t right_operand);


void execute_mult();


void multiply_rm_rs();
void accumulate_rm_rs_rn();
void register_select_write(uint32_t calc, uint32_t reg);
uint32_t register_select_read(uint32_t reg);

void execute_single_data_trans();



void execute_branch();


void instr_decode(uint32_t instr);
void instr_execute(uint32_t instr);

void cpu_cycle();

void print_registers(void);

#endif
