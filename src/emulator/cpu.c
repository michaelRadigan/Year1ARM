#include "common_instances.h"


/* Global variables */


/**
 * Pointer Definitions 
 */
cpu *cpu_ptr;
instr_flags *instr_flags_ptr;


/**
 * Carries out the fetch, decode, execute cycle
 * @param cpu Pointer to the cpu
 */
void
cpu_cycle(void){

  while(1){
    if(pipeline_push(cpu_ptr->pc) == 1){
      uint32_t instr = pipeline_pop();

      if(!instr){
        cpu_ptr->pc += 4;
        print_registers();
        break;
      }
      else{
        instr_decode(instr);
        instr_execute(instr);
        cpu_ptr->pc += 4;
      }
    }
  }
}


/**
 * The pipeline structure
 */
int 
pipeline_push(uint32_t pc){
  pipeline_ptr->decoded = pipeline_ptr->fetched;
  
  uint32_t instr = memory_fetch_word(pc);
  pipeline_ptr->fetched = instr;

  return pipeline_ptr->decoded != UNDEFINED;
}


/**
 * Pops the instruction to be executed next in the pipeline
 */
uint32_t 
pipeline_pop(void) {
  uint32_t decoded = pipeline_ptr->decoded;
  
  pipeline_ptr->decoded = pipeline_ptr->fetched;
  pipeline_ptr->fetched = UNDEFINED;
  
  return decoded;
}


/**
 * Selects which instruction to decode
 * @param instr The instruction word to be decoded
 */
void
instr_decode(uint32_t instr){
	
  if(instr_mult(instr)){
    decode_mult(instr);
  }
  else if(instr_single_data_trans(instr)){
    decode_single_data_trans(instr);
  }
  else if(instr_branch(instr)){
    decode_branch(instr);
  }
  else if(instr_data_proc(instr)){
    decode_data_proc(instr);
  }
  else{
    printf("Cannot decode unsupported instruction\n");
  }
}


/**
 * Selects which instruction to execute
 * @param instr The instruction word to be executed
 */
void
instr_execute(uint32_t instr){
	
  if(instr_mult(instr)){
    if (check_instr_cond_code(instr_mult_ptr->cond)){
      execute_mult();
    }
  }
  else if(instr_single_data_trans(instr)){
    if(check_instr_cond_code(instr_single_data_trans_ptr->cond)){
      execute_single_data_trans();
    }
  }
  else if(instr_branch(instr)){
    if(check_instr_cond_code(instr_branch_ptr->cond)){
      execute_branch();
    }
  }
  else if(instr_data_proc(instr)){
    if(check_instr_cond_code(instr_data_proc_ptr->cond)){
      execute_data_proc();
    }
  }
  else{
    printf("Cannot execute unsupported instruction\n");
  }
}


/* Boolean methods to check validity */


/**
 * Checks for the chosen bit pattern
 * @param instr The word instruction
 * @param offset Amount to shift bit-mask
 * @param shift  Amount to shift the extracted bits
 * @param expected Expected bit pattern
 */
int
check_bits(uint32_t instr, uint32_t mask, int shift, uint32_t expected){
  return ((instr & mask) >> shift) == expected;
}


/**
 * Extracts the relevant bit selection
 * @param instr The word instruction
 * @param mask  The relevant bit mask
 * @param shift The relevant shift amount
 */
uint32_t
extract_bits(uint32_t instr, uint32_t mask, uint32_t shift){
  return ((instr & mask) >> shift); 
}


/**
 * Checks if instruction is Data Processing
 * @param instr The instruction word
 */
int
instr_data_proc(uint32_t instr){
  return check_bits(instr, BITS_DATA_PROC_MASK, 26, 0x00000000);
}


/**
 * Checks if instruction is Multiply
 * @param instr The instruction word
 */
int
instr_mult(uint32_t instr){
  return check_bits(instr, BITS_MULT_PTRN_MASK_1, 4, 0x00000009) &&
         check_bits(instr, BITS_MULT_PTRN_MASK_2, 22, 0x00000000);
}


/**
 * Checks if instruction is Single Data Transfer
 * @param instr The instruction word
 */
int
instr_single_data_trans(uint32_t instr){
  return check_bits(instr, BITS_SINGLE_DATA_TRANS_MASK_1, 21, 0x00000000) &&
         check_bits(instr, BITS_SINGLE_DATA_TRANS_MASK_2, 26, 0x00000001);
}


/**
 * Checks if instruction is Branch
 * @param instr The instruction word
 */
int
instr_branch(uint32_t instr){
  return check_bits(instr, BITS_BRANCH_MASK, 24, 0x0000000A);
}


/**
 * Checks if the Condition field of the instruction is 
 * satisfied by the CPSR register or if field is 1110 (al)
 * @param instr The instruction condition
 */
int 
check_instr_cond_code(uint32_t cond_instr){

  switch(cond_instr){
    case CC_EQ : return instr_flags_ptr->flag_Z == 1;
    case CC_NE : return instr_flags_ptr->flag_Z == 0;
    case CC_GE : return instr_flags_ptr->flag_N == instr_flags_ptr->flag_V;
    case CC_LT : return instr_flags_ptr->flag_N != instr_flags_ptr->flag_V;
    case CC_GT : return instr_flags_ptr->flag_Z == 0 && (instr_flags_ptr->flag_N 
	                                                     == instr_flags_ptr->flag_V);		
    case CC_LE : return instr_flags_ptr->flag_Z == 1 || (instr_flags_ptr->flag_N 
	                                                     != instr_flags_ptr->flag_V);
    case CC_AL : return 1;
    default : printf("Condition Code not supported\n");
  }
  return 0;
}


/* Decoding of instructions */


/**
 * Extracts the relevant bits and sets them in the Data Proc struct
 * @param instr The instruction word
 */
void 
decode_data_proc(uint32_t instr){

  //Cond
  uint32_t cond_val = extract_bits(instr, BITS_COND_MASK, 28);
  instr_data_proc_ptr->cond = cond_val;

  //I flag
  uint32_t I_flag = extract_bits(instr, FLAG_I_MASK, 25);
  instr_data_proc_ptr->I_flag = I_flag;

  //Opcode  //Deallocate memory

  /*
  memory_machine_destroy(&memory);
  memory_cpu_destroy();
  memory_instr_data_proc_destroy();
  memory_instr_mult_destroy();
  memory_instr_single_data_transfer_destroy();
  memory_instr_branch_destroy();
*/
  uint32_t op_code = extract_bits(instr, OPCODE_MASK, 21);
  instr_data_proc_ptr->op_code = op_code;
	
  //S flag
  uint32_t S_flag = extract_bits(instr, FLAG_S_MASK, 20);
  instr_data_proc_ptr->S_flag = S_flag;
	
  //Rn reg
  uint32_t rn_reg = extract_bits(instr, REG_1_MASK, 16);
  instr_data_proc_ptr->rn_reg = rn_reg;
	  //Deallocate memory

  /*
  memory_machine_destroy(&memory);
  memory_cpu_destroy();
  memory_instr_data_proc_destroy();
  memory_instr_mult_destroy();
  memory_instr_single_data_transfer_destroy();
  memory_instr_branch_destroy();
*/
  //Rd reg
  uint32_t rd_reg = extract_bits(instr, REG_2_MASK, 12);
  instr_data_proc_ptr->rd_reg = rd_reg;
	
  //Operand2
  uint32_t operand_2 = extract_bits(  //Deallocate memory

  /*
  memory_machine_destroy(&memory);
  memory_cpu_destroy();
  memory_instr_data_proc_destroy();
  memory_instr_mult_destroy();
  memory_instr_single_data_transfer_destroy();
  memory_instr_branch_destroy();
*/instr, OPERAND_MASK, 0);  //Deallocate memory

  /*
  memory_machine_destroy(&memory);
  memory_cpu_destroy();
  memory_instr_data_proc_destroy();
  memory_instr_mult_destroy();
  memory_instr_single_data_transfer_destroy();
  memory_instr_branch_destroy();
*/
  instr_data_proc_ptr->operand_2 = operand_2;
}


/**
 * Extracts the relevant bits and sets them in the Mult struct
 * @param instr The instruction word
 */
void
decode_mult(uint32_t instr){

  //Cond
  uint32_t cond_val = extract_bits(instr, BITS_COND_MASK, 28);
  instr_mult_ptr->cond = cond_val;
	
  //A flag
  uint32_t A_flag = extract_bits(instr, FLAG_A_MASK, 21);
  instr_mult_ptr->A_flag = A_flag;

  //S flag
  uint32_t S_flag = extract_bits(instr, FLAG_S_MASK, 20);
  instr_mult_ptr->S_flag = S_flag;	
	
  //Rd reg
  uint32_t rd_reg = extract_bits(instr, REG_1_MASK, 16);
  instr_mult_ptr->rd_reg = rd_reg;
	
  //Rn reg
  uint32_t rn_reg = extract_bits(instr, REG_2_MASK, 12);
  instr_mult_ptr->rn_reg = rn_reg;	
	
  //Rs reg
  uint32_t rs_reg = extract_bits(instr, REG_3_MASK, 8);
  instr_mult_ptr->rs_reg = rs_reg;	
	
  //Rm reg
  uint32_t rm_reg = extract_bits(instr, REG_4_MASK, 0);
  instr_mult_ptr->rm_reg = rm_reg;
}


/**
 * Extracts the relevant bits and sets them in the Single Data Trans struct
 * @param instr The instruction word
 */
void
decode_single_data_trans(uint32_t instr){

  //Cond
  uint32_t cond_val = extract_bits(instr, BITS_COND_MASK, 28);
  instr_single_data_trans_ptr->cond = cond_val;

  //I flag
  uint32_t I_flag = extract_bits(instr, FLAG_I_MASK, 25); 
  instr_single_data_trans_ptr->I_flag = I_flag;
	
  //P flag
  uint32_t P_flag = extract_bits(instr, FLAG_P_MASK, 24);
  instr_single_data_trans_ptr->P_flag = P_flag;
	
  //U flag
  uint32_t U_flag = extract_bits(instr, FLAG_U_MASK, 23);
  instr_single_data_trans_ptr->U_flag = U_flag;
	
  //L flag
  uint32_t L_flag = extract_bits(instr, FLAG_L_MASK, 20);
  instr_single_data_trans_ptr->L_flag = L_flag;	
	
  //Rn reg
  uint32_t rn_reg = extract_bits(instr, REG_1_MASK, 16);
  instr_single_data_trans_ptr->rn_reg = rn_reg;
	
  //Rd reg
  uint32_t rd_reg = extract_bits(instr, REG_2_MASK, 12);
  instr_single_data_trans_ptr->rd_reg = rd_reg;	
	
  //Offset
  uint32_t offset = extract_bits(instr, OFFSET_1_MASK, 0);
  instr_single_data_trans_ptr->offset = offset;
}


/**
 * Extracts the relevant bits and sets them in the Branch struct
 * @param instr The instruction word
 */
void
decode_branch(uint32_t instr){

  //Cond
  uint32_t cond_val = extract_bits(instr, BITS_COND_MASK, 28);
  instr_branch_ptr->cond = cond_val;

  //Offset
  uint32_t offset = extract_bits(instr, OFFSET_2_MASK, 0);
  instr_branch_ptr->offset = offset;
}
 

/* Execution of instructions */


/**
 * Executes Data Processing Instruction
 */
void
execute_data_proc(void){
	
  //Extract relevant instruction fields
  uint32_t operand_2_val = result_set_I_flag(I_flag_set_data_proc(), instr_data_proc_ptr->operand_2);    
  uint32_t reg_val = instr_data_proc_ptr->rn_reg;
  uint32_t reg_contents = register_select_read(reg_val);
  uint32_t op_code = instr_data_proc_ptr->op_code;	
  uint32_t result = opcode_dispatch(op_code, reg_contents, operand_2_val);

  if(S_flag_set_data_proc()){
    //Update CPSR flags

    //Set C flag 
    C_flag_set(op_code);

    //Set Z flag 
    if(result == 0x0){
      instr_flags_ptr->flag_Z = 1;
    }
    update_CPSR();
  }
  uint32_t rd_reg = instr_data_proc_ptr->rd_reg;
	
  register_select_write_opcode(op_code, result, rd_reg);
}


/**
 * Returns the manipulated Operand2/Offset value depending on the I flag
 * @param I_flag_set One or zero depending on whether flag is set
 * @param operand_2_or_offset Either field from Data Proc and Single Data Trans
 */
uint32_t
result_set_I_flag(int I_flag_set, uint32_t operand_2_or_offset){	
	
  uint32_t result_val = 0;
     
  if(I_flag_set){
    //Operand2/Offset is an immediate constant 
    uint32_t immediate_value = extract_bits(operand_2_or_offset, IMMEDIATE_VALUE_MASK, 0);
    uint32_t rotate_field    = extract_bits(operand_2_or_offset, ROTATE_FIELD_MASK, 8);

    //Double rotation amount
    uint32_t rotate_amount = rotate_field << 1;
		
    //Rotate right 
    uint32_t rotated_result = execute_rotate_right(rotate_amount, immediate_value);

    result_val = rotated_result;
  }
  else{
    //Operand2/Offset is a shifted register 

    //Read contents of register Rm 
    uint32_t reg_rm = extract_bits(operand_2_or_offset, RM_REG_MASK, 0);
    uint32_t reg_val = register_select_read(reg_rm);

    //Examine bit 5 of Operand2/Offset
    uint32_t bit_4 = extract_bits(operand_2_or_offset, BIT_4_MASK, 4);

    //Extract shift type 
    uint32_t shift_type  = extract_bits(operand_2_or_offset, SHIFT_TYPE_MASK, 5);
		
    if(bit_4){//If bit 4 == 1 then shift is specified by register 

    //Read contents of register Rs
    uint32_t reg_rs = extract_bits(operand_2_or_offset, RS_REG_MASK, 8);
    uint32_t reg_rs_val = register_select_read(reg_rs);

    //Extract bottom byte of Rs register
    uint32_t bottom_byte = extract_bits(reg_rs_val, RS_VAL_MASK, 0);

    result_val = shift_type_dispatch(shift_type, bottom_byte, reg_val);
    }
    else{//Else shift by a constant amount

      //Integer shift value
      uint32_t shift_amount = extract_bits(operand_2_or_offset, SHIFT_VALUE_MASK, 7);

      result_val = shift_type_dispatch(shift_type, shift_amount, reg_val);
    }
  }
  return result_val;
}


/**
 * Carries out the Multiply Instruction
 */
void
execute_mult(void){
	
  if(A_flag_set()){
    //Perform multiply and accumulate 
    multiply_rm_rs();
    accumulate_rm_rs_rn();
  }
  else{
    //Perform multiply 
    multiply_rm_rs();
  }

  //If S bit is set, update N and Z flags of CPSR
  if(S_flag_set_mult()){
    uint32_t rd_reg = instr_mult_ptr->rd_reg;
    uint32_t result = register_select_read(rd_reg);

    //Set Z flag iff result is 0
    if(result == 0){
      instr_flags_ptr->flag_Z = 1;
    }

    //Set N flag to bit 31 of result 
    uint32_t flag_N_result = extract_bits(result, 0x40000000, 30);
    instr_flags_ptr->flag_N = flag_N_result;
  }
  update_CPSR();
}


/**
 * Carries out the Single Data Transfer Instruction 
 */
void
execute_single_data_trans(void){

  //The opposite is true for data processing instructions
  uint32_t offset_value = result_set_I_flag(!I_flag_set_single_data_trans(), instr_single_data_trans_ptr->offset);

  //Extract relevant instruction fields
  uint32_t base_reg = instr_single_data_trans_ptr->rn_reg;
  uint32_t base_reg_contents = register_select_read(base_reg);
	
  uint32_t s_or_d_reg = instr_single_data_trans_ptr->rd_reg;
  uint32_t s_or_d_reg_contents = register_select_read(s_or_d_reg);
	
  uint32_t memory_access_index = base_reg_contents;

  if(P_flag_set()){
    //Pre-Indexing
    //Offset is added/subtracted to the base register before transferring the data.

    if(U_flag_set()){
      //Offset is added to base register 
      memory_access_index = base_reg_contents + offset_value;
    }
    else{
      //Offset is subtracted from base register
      memory_access_index = base_reg_contents - offset_value;
    }
    transfer_data(memory_access_index, s_or_d_reg, s_or_d_reg_contents);
  }
  else{
    //Post-Indexing
    //Offset is added/subtracted to the base register after transferring the data.
	
    transfer_data(memory_access_index, s_or_d_reg, s_or_d_reg_contents);
        
    if(U_flag_set()){
      //Offset is added to base register
      memory_access_index = base_reg_contents + offset_value;
    }
    else{
      //Offset is subtracted from base register
      memory_access_index = base_reg_contents - offset_value;
    }
    register_select_write(memory_access_index, base_reg);
  }
  update_CPSR();
}



/**
 * Carries out the Branch Instruction
 */
void
execute_branch(void){

  uint32_t result = 0; 
  uint32_t offset = instr_branch_ptr->offset;

  //Shift offset left 2 bits 
  uint32_t offset_shift = offset << 2;

  //Sign extend shifted offset
  uint32_t ms_bit = extract_bits(offset_shift, BIT_26_MASK, 25);

  if(ms_bit){
    uint32_t sign_extend_1_mask = 0xFC000000;
    result = sign_extend_1_mask | offset_shift; 	
  }
  else{
    result = offset_shift;
  }

  cpu_ptr->pc += (int32_t) result + 4;
  update_CPSR();
  pipeline_ptr->fetched = UNDEFINED;
}


/**
 * Writes the result of the calculation to the correct register
 */
void
register_select_write(uint32_t calc, uint32_t reg){

  switch(reg){
    case R0  : cpu_ptr->r0 = calc; break;
    case R1  : cpu_ptr->r1 = calc; break;
    case R2  : cpu_ptr->r2 = calc; break;
    case R3  : cpu_ptr->r3 = calc; break;
    case R4  : cpu_ptr->r4 = calc; break;
    case R5  : cpu_ptr->r5 = calc; break;
    case R6  : cpu_ptr->r6 = calc; break;
    case R7  : cpu_ptr->r7 = calc; break;
    case R8  : cpu_ptr->r8 = calc; break;
    case R9  : cpu_ptr->r9 = calc; break;
    case R10 : cpu_ptr->r10 = calc; break;
    case R11 : cpu_ptr->r11 = calc; break;
    case R12 : cpu_ptr->r12 = calc; break;
    default :printf("Invalid reg write\n");
  }
}


/**
 * Reads the content of the selected register
 */
uint32_t 
register_select_read(uint32_t reg){

  switch(reg){
    case R0  : return cpu_ptr->r0; 
    case R1  : return cpu_ptr->r1; 
    case R2  : return cpu_ptr->r2; 
    case R3  : return cpu_ptr->r3; 
    case R4  : return cpu_ptr->r4; 
    case R5  : return cpu_ptr->r5; 
    case R6  : return cpu_ptr->r6; 
    case R7  : return cpu_ptr->r7; 
    case R8  : return cpu_ptr->r8; 
    case R9  : return cpu_ptr->r9; 
    case R10 : return cpu_ptr->r10; 
    case R11 : return cpu_ptr->r11; 
    case R12 : return cpu_ptr->r12; 
    case PC  : return cpu_ptr->pc + 8;
    default :printf("Invalid reg read\n");
  }
  return 0;
}


/**
 * Writes to the destination register, depending on the opcode
 * @param op_code The instruction opcode
 * @param result  The result which has to be written (or not)
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
    case OC_TST : /* Don't write */ break;
    case OC_TEQ : /* Don't write */ break;
    case OC_CMP : /* Don't write */ break;
    case OC_ORR : register_select_write(result, rd_reg); break;
    case OC_MOV : register_select_write(result, rd_reg); break;
    default : printf("Opcode reg write error");
  }
}


/**
 * Transfers data to/from memory
 * @param memory_access_index The index into memory
 * @param s_or_d_reg          The source or destination register
 * @param s_or_d_reg_contents The contents of the source or destination register
 */
void
transfer_data(uint32_t memory_access_index, uint32_t s_or_d_reg, uint32_t s_or_d_reg_contents){

  if(L_flag_set()){
  //Write GPIO to register or load word into register

    if(gpio_memory_location(memory_access_index)){ 		
      register_select_write(memory_access_index, s_or_d_reg);
        return;
    }
    else{ 
      word_load(memory_access_index, s_or_d_reg);
    }
  }  
  else{
  //Check GPIO and store word into memory
		
    if(gpio_memory_location(memory_access_index)){ 		
      return;
    }
    else{ 
      word_store(memory_access_index, s_or_d_reg_contents);
    }
  }
}


/**
 * Selects which shift operation should be executed and returns 
 * the result after shift operation is carried out
 * @param shift_type   The type of shift operation 
 * @param shift_amount The amount the value should be shifted by
 * @param reg_val      The value stored in the register
 */
uint32_t
shift_type_dispatch(uint32_t shift_type, uint32_t shift_amount, uint32_t reg_val){

  switch(shift_type){
    case LSL : return execute_shift_type(&execute_logical_shift_left, shift_amount, reg_val);
    case LSR : return execute_shift_type(&execute_logical_shift_right, shift_amount, reg_val);
    case ASR : return execute_shift_type(&execute_arithmetic_shift_right, shift_amount, reg_val);
    case ROR : return execute_shift_type(&execute_rotate_right, shift_amount, reg_val);
    default  : printf("Given shift type not supported");
  }
  return 0;
}


/**
 * Passes the given function pointer and parameters to the selected shift operation
 * @param (*execute_shift_type_ptr)(uint32_t, uint32_t) Shift types function pointer
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
    case OC_AND : return execute_op_code(&execute_op_code_and, left_operand, right_operand);
    case OC_EOR : return execute_op_code(&execute_op_code_eor, left_operand, right_operand);
    case OC_SUB : return execute_op_code(&execute_op_code_sub, left_operand, right_operand);
    case OC_RSB : return execute_op_code(&execute_op_code_rsb, left_operand, right_operand);
    case OC_ADD : return execute_op_code(&execute_op_code_add, left_operand, right_operand);
    case OC_TST : return execute_op_code(&execute_op_code_tst, left_operand, right_operand);
    case OC_TEQ : return execute_op_code(&execute_op_code_teq, left_operand, right_operand);
    case OC_CMP : return execute_op_code(&execute_op_code_cmp, left_operand, right_operand);
    case OC_ORR : return execute_op_code(&execute_op_code_orr, left_operand, right_operand);
    case OC_MOV : return execute_op_code(&execute_op_code_mov, left_operand, right_operand);
    default : printf("Unsupported Opcode selected");
  }
  return 0;
}


/**
 * Passes the given function pointer and parameters to the selected opcode
 * @param  uint32_t (*execute_op_code_ptr)(uint32_t, uint32_t)
 *     opcode function pointer
 * @param left_operand  A value used when carrying out the opcode
 * @param right_operand A value used when carrying out the opcode
 * @return The result of carrying out the operation to the top level
 */
uint32_t
execute_op_code(uint32_t (*execute_op_code_ptr)(uint32_t, uint32_t), 
		uint32_t left_operand, uint32_t right_operand){
  return (*execute_op_code_ptr)(left_operand, right_operand);
}


/**
 * Writes the word at the memory access index into the selected register
 * @param memory_access_index The index into memory 
 * @param s_or_d_reg          The destination register
 */
void
word_load(uint32_t memory_access_index, uint32_t s_or_d_reg){

  if(memory_access_index > MEM_SIZE){
    printf("Error: Out of bounds memory access at address 0x%08x\n", memory_access_index);
  }
  else{
    //Load word into memory
    uint32_t word_load = memory_fetch_word(memory_access_index);
    register_select_write(word_load, s_or_d_reg);
  }
}


/**
 * Writes into memory the contents of a register
 * @param memory_access_index The index into memory 
 * @param s_or_d_contents     The contents of the selected register
 */
void 
word_store(uint32_t memory_access_index, uint32_t s_or_d_reg_contents){

  if(memory_access_index > MEM_SIZE){
    printf("Error: Out of bounds memory access at address 0x%08x\n", memory_access_index);
  }
  else{
    //Store word into memory
    uint32_t word_store = s_or_d_reg_contents;
    memory_write_word(memory_access_index, word_store);
  }
}


/**
 * Prints out the appropriate message depending on the memory access index
 * @param memory_access_index The index into memory
 * @return Used at a higher level so return 1/0
 */
int
gpio_memory_location(uint32_t memory_access_index){

  switch(memory_access_index){
    case GPIO_1 : printf("One GPIO pin from 0 to 9 has been accessed\n"); return 1;
    case GPIO_2 : printf("One GPIO pin from 10 to 19 has been accessed\n"); return 1;
    case GPIO_3 : printf("One GPIO pin from 20 to 29 has been accessed\n"); return 1;
    case GPIO_CLEAR : printf("PIN OFF\n"); return 1;
    case GPIO_SET   : printf("PIN ON\n");  return 1;
    default : return 0;
  }
  return 0;
}


/**
 * Prints the values of each of the registers
 */
void
print_registers(void){
  printf("%s", "Registers:\n");
  printf("$0  : %10d (0x%08x)\n", cpu_ptr->r0, cpu_ptr->r0);
  printf("$1  : %10d (0x%08x)\n", cpu_ptr->r1, cpu_ptr->r1);
  printf("$2  : %10d (0x%08x)\n", cpu_ptr->r2, cpu_ptr->r2);
  printf("$3  : %10d (0x%08x)\n", cpu_ptr->r3, cpu_ptr->r3);
  printf("$4  : %10d (0x%08x)\n", cpu_ptr->r4, cpu_ptr->r4);
  printf("$5  : %10d (0x%08x)\n", cpu_ptr->r5, cpu_ptr->r5);
  printf("$6  : %10d (0x%08x)\n", cpu_ptr->r6, cpu_ptr->r6);
  printf("$7  : %10d (0x%08x)\n", cpu_ptr->r7, cpu_ptr->r7);
  printf("$8  : %10d (0x%08x)\n", cpu_ptr->r8, cpu_ptr->r8);
  printf("$9  : %10d (0x%08x)\n", cpu_ptr->r9, cpu_ptr->r9);
  printf("$10 : %10d (0x%08x)\n", cpu_ptr->r10, cpu_ptr->r10);
  printf("$11 : %10d (0x%08x)\n", cpu_ptr->r11, cpu_ptr->r11);
  printf("$12 : %10d (0x%08x)\n", cpu_ptr->r12, cpu_ptr->r12);
  printf("PC  : %10d (0x%08x)\n", cpu_ptr->pc + 4, cpu_ptr->pc + 4);
  printf("CPSR: %10d (0x%08x)\n", cpu_ptr->cpsr, cpu_ptr->cpsr);
  printf("Non-zero memory:\n");

  for(int i = 0; i < MEM_SIZE; i += 4){
    uint32_t mem_contents = memory_fetch_word(i);
    uint32_t mem_contents_converted = memory_swap_word_be_to_le(mem_contents);
    if(mem_contents_converted){
      printf("0x%08x: 0x%08x\n", i, mem_contents_converted);
    }
  }
}
