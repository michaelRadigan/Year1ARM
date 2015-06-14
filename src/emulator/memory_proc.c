#include "common_instances.h"


/* Pointer declarations for structs*/


memory_machine *memory;
pipeline_obj *pipeline_ptr;

instr_data_proc_struct *instr_data_proc_ptr;
instr_mult_struct *instr_mult_ptr;
instr_single_data_trans_struct *instr_single_data_trans_ptr;
instr_branch_struct *instr_branch_ptr;

/**
 * Converts a word from Big Endian to Little Endian
 * @param word The word to be converted
 */
uint32_t
memory_swap_word_be_to_le(uint32_t word){
  word = ((word << 8) & 0xFF00FF00) | ((word >> 8) & 0x00FF00FF);
  return (word << 16) | (word >> 16);
}


/**
 * Fetches 4 bytes from memory and concatenates into 1 word
 * @param pc Program counter acts as index to memory
 */
uint32_t
memory_fetch_word(uint32_t pc){

  uint8_t byteOne   = memory->byte[pc];
  uint8_t byteTwo   = memory->byte[pc + 1];
  uint8_t byteThree = memory->byte[pc + 2];
  uint8_t byteFour  = memory->byte[pc + 3];

  //Do bit-wise shifting to concatenate the four bytes together
  //Does automatic conversion from Little to Big Endian
  return byteOne | (byteTwo << 8) | (byteThree << 16) | (byteFour << 24);
}


/**
 * Writes the given word to a position in memory
 */
void
memory_write_word(uint16_t memory_index, uint32_t word_to_write){

  //Does conversion from Big to Little endian for memory write
  uint32_t mask = 0x000000FF;

  for(int i = 0; i < 4; i++){
    memory->byte[memory_index + i] = (word_to_write & mask) >> i * 8;
    mask <<= 8;
  }
}


/**
 * Sets up machine memory
 * @param memptr Pointer to machine memory
 */
void
memory_machine_init(memory_machine **memptr){

  int numBytes = MEM_SIZE;
	
  *memptr = calloc(1, sizeof(memory_machine));
  (*memptr)->byte = calloc(numBytes, sizeof(uint8_t));
}


/**
 * Frees machine memory
 * @param memptr Pointer to macine memory
 */
void
memory_machine_destroy(memory_machine **memptr){
  free((*memptr)->byte);
  free(*memptr);
}


/**
 * Sets up CPU 
 */
void
memory_cpu_init(void){
  cpu_ptr = calloc(1, sizeof(cpu));
}


/**
 * Frees CPU
 */
void
memory_cpu_destroy(void){
  free(cpu_ptr);
}


/**
 * Sets up instruction memory
 */
void
memory_instr_data_proc_init(void){
  instr_data_proc_ptr = calloc(1, sizeof(instr_data_proc_struct));
}


/**
 * Sets up instruction memory
 */
void
memory_instr_mult_init(void){
  instr_mult_ptr = calloc(1, sizeof(instr_mult_struct));
}


/**
 * Sets up instruction memory
 */
void
memory_instr_single_data_transfer_init(void){
  instr_single_data_trans_ptr = calloc(1, sizeof(instr_single_data_trans_struct));
}


/**
 * Sets up instruction memory
 */
void
memory_instr_branch_init(void){
  instr_branch_ptr = calloc(1, sizeof(instr_branch_struct));
}


/**
 * Frees instruction memory
 */
void
memory_instr_data_proc_destroy(void){
  free(instr_data_proc_ptr);
}


/**
 * Frees instruction memory
 */
void
memory_instr_mult_destroy(void){
  free(instr_mult_ptr);
}


/**
 * Frees instruction memory
 */
void
memory_instr_single_data_transfer_destroy(void){
  free(instr_single_data_trans_ptr);
}


/**
 * Frees instruction memory
 */
void
memory_instr_branch_destroy(void){
  free(instr_branch_ptr);
}


/**
 * Sets up flag struct
 */
void
memory_instr_flags_init(void){
  instr_flags_ptr = calloc(1, sizeof(instr_flags));
}


/**
 * Frees flag struct memory
 */
void
memory_instr_flags_destroy(void){
  free(instr_flags_ptr);
}


/**
 * Sets up the pipeline
 */
void
memory_pipeline_init(void){
  pipeline_ptr = calloc(1, sizeof(pipeline_obj));
  pipeline_ptr->fetched = UNDEFINED;
  pipeline_ptr->decoded = UNDEFINED;
}


/*
 * Frees the pipeline
 */
void
memoyr_pipeline_destroy(void){
  free(pipeline_ptr);
}


/**
 * Checks whether EOF or an error has occured
 * @param file Pointer to the binary file
 */
void
check_file_error(FILE *file){
  if(ferror(file)){
    printf("File error\n");
  }
}


/**
 * Loads a binary file into the application
 * @param file Pointer to the binary file
 */
void 
memory_load_file(FILE *file){

  memory = NULL;
  memory_machine_init(&memory);
  memory_instr_flags_init();
  memory_cpu_init();
  memory_instr_data_proc_init();
  memory_instr_mult_init();
  memory_instr_single_data_transfer_init();
  memory_instr_branch_init();
  memory_pipeline_init();
	
  for(int i = 0; i < MEM_SIZE; i++){
    if(fread(memory->byte + i, BYTES, 1, file) == 1){
      continue;
    }
    else{
      check_file_error(file);
      break;
    }
  }


  cpu_cycle();
  //Deallocate memory


  memory_instr_data_proc_destroy();
  memory_instr_mult_destroy();
  memory_instr_single_data_transfer_destroy();
  memory_instr_branch_destroy();
  memory_cpu_destroy();
  memoyr_pipeline_destroy();
  memory_instr_flags_destroy();
  memory_machine_destroy(&memory);



  fclose(file);
}
