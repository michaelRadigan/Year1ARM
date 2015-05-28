#ifndef CPU_H
#define CPU_H

#include "common_instances.h"


typedef struct{
	
//<<<<<<< HEAD
	uint32_t *cpu;

	uint32_t decoded;
	uint32_t fetched;

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
	uint32_t rsp;
	uint32_t lr;
	uint32_t pc;
	uint32_t cpsr;
}cpu;

extern cpu *cpu_ptr;


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
 * Flags
 */
typedef struct{
	/* All flags are 1 bit wide */ 
	
	/* Instruction Flags */
	int flag_I : 1;
	int flag_S : 1;
	int flag_A : 1;
	int flag_P : 1;
	int flag_U : 1;
	int flag_L : 1;

	/* CSPR register flags */
	int flag_N : 1;
	int flag_Z : 1;
	int flag_C : 1;
	int flag_V : 1;

	/* Carry flag used in arithmetic and shift operation */
	int flag_carry : 1;

}instr_flags;

extern instr_flags *instr_flags_ptr;


/**
 * CPU registers
 */
typedef enum{
	R0 = 0x0,
	R1 = 0x1,
	R2 = 0x2,
	R3 = 0x3,
	R4 = 0x4,
	R5 = 0x5,
	R6 = 0x6,
	R7 = 0x7,
	R8 = 0x8,
	R9 = 0x9,
	R10 = 0xA,
	R11 = 0xB,
	R12 = 0xC,
	RSP = 0xD,
	RLR = 0xE,
	RPC = 0xF,
	CPSR = 0x10
}cpu_reg;


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

void execute_data_proc();
void C_flag_set(uint32_t op_code);
void register_select_write_opcode(uint32_t opcode, uint32_t result, uint32_t rd_reg);
uint32_t shift_type_dispatch(uint32_t shift_type, uint32_t shift_amount, uint32_t reg_val);
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
static int I_flag_set(); 


void execute_mult();
static void multiply_rm_rs();
static void accumulate_rm_rs_rn();
void register_select_write(uint32_t calc, uint32_t reg);
uint32_t register_select_read(uint32_t reg);
static int A_flag_set(void);
static int S_flag_set(void);


void execute_single_data_trans();


void execute_branch();

void instr_decode(uint32_t instr);
void instr_execute(uint32_t instr);

void cpu_cycle(cpu *cpu);

#endif
