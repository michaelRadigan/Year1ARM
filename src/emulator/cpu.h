#ifndef CPU_H
#define CPU_H


#include "common_instances.h"


#define BITS_DATA_PROC_MASK           0x0C000000
#define BITS_MULT_PTRN_MASK_1         0x000000F0
#define BITS_MULT_PTRN_MASK_2         0x0FC00000
#define BITS_SINGLE_DATA_TRANS_MASK_1 0x00600000
#define BITS_SINGLE_DATA_TRANS_MASK_2 0x0C000000
#define BITS_BRANCH_MASK              0x0A000000
#define BITS_COND_MASK                0xF0000000

#define FLAG_I_MASK                   0x02000000
#define FLAG_A_MASK                   0x00200000
#define FLAG_P_MASK                   0x01000000
#define FLAG_U_MASK                   0x00800000
#define OPCODE_MASK                   0x01E00000
#define FLAG_S_MASK                   0x00100000
#define FLAG_L_MASK                   0x00100000
#define REG_1_MASK                    0x000F0000
#define REG_2_MASK                    0x0000F000
#define REG_3_MASK                    0x00000F00
#define REG_4_MASK                    0x0000000F
#define OPERAND_MASK                  0x00000FFF 
#define OFFSET_1_MASK                 0x00000FFF 
#define OFFSET_2_MASK                 0x00FFFFFF

#define IMMEDIATE_VALUE_MASK          0x000000FF
#define ROTATE_FIELD_MASK             0x00000F00
#define BIT_4_MASK                    0x00000010
#define RM_REG_MASK                   0x0000000F
#define SHIFT_VALUE_MASK              0x00000F80
#define SHIFT_TYPE_MASK               0x00000060
#define RS_REG_MASK                   0x00000F00
#define RS_VAL_MASK                   0x000000FF
#define RESULT_BIT_MASK               0x80000000

#define BIT_26_MASK                   0x02000000

#define UNDEFINED                     0xFFFFFFFF
#define PIPELINE_SIZE                 2


/**
 * Pipeline structure
 */
typedef struct {
  uint32_t fetched;
  uint32_t decoded;
}pipeline_obj;

extern pipeline_obj *pipeline_ptr;


/**
 * CPU structure
 */
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
  uint32_t sp;//Ignored in this implementation
  uint32_t lr;//Ignored in this implementation
  uint32_t pc;
  uint32_t cpsr; 
}cpu;

extern cpu *cpu_ptr;


/**
 * Flags
 */
typedef struct{
  //Instruction Flags 
  int flag_I;
  int flag_S;
  int flag_A;
  int flag_P;
  int flag_U;
  int flag_L;

  //CSPR register flags
  int flag_N;//The last result was negative
  int flag_Z;//The last result was zero
  int flag_C;//The last result caused a bit to be carried out
  int flag_V;//The last result overflowed

  //Carry flag used in arithmetic and shift operation
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
  // Continuity of OpCodes is not maintained because we 
  // are only dealing with a subset of them
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
  SP   = 0xD,// Is ignored in this implementation
  LR   = 0xE,// Is ignored in this implementation
  PC   = 0xF,
  CPSR = 0x10
}cpu_reg;


/**
 * GPIO pins
 */
typedef enum{
  GPIO_1 = 0x20200000,
  GPIO_2 = 0x20200004,
  GPIO_3 = 0x20200008,

  GPIO_CLEAR = 0x20200028,
  GPIO_SET   = 0x2020001C
}gpio;


/* Function prototypes */


void cpu_cycle(void);
int pipeline_push(uint32_t pc);
uint32_t pipeline_pop(void);

void instr_decode(uint32_t instr);
void instr_execute(uint32_t instr);

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

void execute_data_proc(void);
uint32_t result_set_I_flag(int I_flag_set, uint32_t operand_2_or_offset);
void execute_mult(void);
void execute_single_data_trans(void);
void execute_branch(void);

void register_select_write(uint32_t calc, uint32_t reg);
uint32_t register_select_read(uint32_t reg);
void register_select_write_opcode(uint32_t opcode, uint32_t result, uint32_t rd_reg);
uint32_t shift_type_dispatch(uint32_t shift_type, uint32_t shift_amount, uint32_t reg_val);
uint32_t execute_shift_type(uint32_t (*execute_shift_type_ptr)(uint32_t, uint32_t),
		uint32_t shift_amount, uint32_t reg_val);
uint32_t opcode_dispatch(uint32_t opcode, uint32_t left_operand, uint32_t right_operand);
uint32_t execute_op_code(uint32_t (*execute_op_code_ptr)(uint32_t, uint32_t),
	    uint32_t left_operand, uint32_t right_operand);


void word_load(uint32_t memory_access_index, uint32_t s_or_d_reg);
void word_store(uint32_t memory_access_index, uint32_t s_or_d_reg_contents);

int gpio_memory_location(uint32_t memory_access_index);

void print_registers(void);


#endif
