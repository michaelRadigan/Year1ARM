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

/*
=======
    uint32_t reg[14]; 

	/*A pointer to the array holding the 15 generaq; purpose registers*
     *we don't need them all but I'm including all 14 for completeness*/
//  uint32_t pc;
//  uint32_t cpsr;
//  uint32_t fetched; /*this will hold the next instruction to be decoded*/
    
    /*this will probably change, just here as a placeholder really*/
//    uint32_t decode; 	
//};

//>>>>>>> 803a795f1453dd24f2b0f53b872e25c083ccee99






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

//extern enum cpu_reg select_reg;

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

void cpu_cycle(cpu *cpu);

#endif
