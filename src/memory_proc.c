#include "common_instances.h"


#define BYTES    1


memory_machine *memory;



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
	/* Do bit-wise shifting to concatenate the four bytes
	 * Does automatic conversion from Little to Big Endian
	 */
	uint32_t result = byteOne | (byteTwo << 8) | (byteThree << 16) | (byteFour << 24);
    return result;
}


void
memory_write_word(uint16_t memory_index, uint32_t word_to_write){
	/* Does conversion from big to little endian for memory write */
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
	
	/* 1 indicates that there is only 1 item in struct 
	 * memory_machine -> may change in future && cause bugs*/
	*memptr = calloc(1, sizeof(memory_machine)); 
	(*memptr)->byte = calloc(numBytes, sizeof(uint8_t));
}


/**
 * Frees machine memory
 * @param memptr Pointer to macine memory
 */
void
memory_machine_destroy(memory_machine *memptr){
	free(memptr);
}


/**
 * Sets up CPU 
 */
void
memory_cpu_init(){
	/* Need to implement with calloc to initialise to zero*/
	/* source of error in calloc due to size of 1 as it can change */
	cpu_ptr = calloc(1, sizeof(struct cpu));
}


/**
 * Frees CPU
 */
void
memory_cpu_destroy(){
	free(cpu_ptr);
}


/* Initialsing memory for instruction struct */

instr_data_proc_struct *instr_data_proc_ptr;
instr_mult_struct *instr_mult_ptr;
instr_single_data_trans_struct *instr_single_data_trans_ptr;
instr_branch_struct *instr_branch_ptr;


/**
 * Sets up instruction memory
 */
void
memory_instr_data_proc_init(){
	/* source of error in calloc due to size of 1 as it can change */
	instr_data_proc_ptr = calloc(1, sizeof(instr_data_proc_struct));
}


/**
 * Sets up instruction memory
 */
void
memory_instr_mult_init(){
	/* source of error in calloc due to size of 1 as it can change */
	instr_mult_ptr = calloc(1, sizeof(instr_mult_struct));

}


/**
 * Sets up instruction memory
 */
void
memory_instr_single_data_transfer_init(){
	/* source of error in calloc due to size of 1 as it can change */
	instr_single_data_trans_ptr = calloc(1, sizeof(instr_single_data_trans_struct));
}


/**
 * Sets up instruction memory
 */
void
memory_instr_branch_init(){
	/* source of error in calloc due to size of 1 as it can change */
	instr_branch_ptr = calloc(1, sizeof(instr_branch_struct));
}



/**
 * Frees instruction memory
 */
void
memory_instr_data_proc_destroy(){
	free(instr_data_proc_ptr);
}


/**
 * Frees instruction memory
 */
void
memory_instr_mult_destroy(){
	free(instr_mult_ptr);
}


/**
 * Frees instruction memory
 */
void
memory_instr_single_data_transfer_destroy(){
	free(instr_single_data_trans_ptr);
}


/**
 * Frees instruction memory
 */
void
memory_instr_branch_destroy(){
	free(instr_branch_ptr);
}


void
memory_instr_flags_init(){
	/* source of error in calloc due to size of 1 as it can change */
	instr_flags_ptr = calloc(1, sizeof(instr_flags));
}

void
memory_instr_flags_destroy(){
	free(instr_flags_ptr);
}


/**
 * Checks whether EOF or an error has occured
 * @param file Pointer to the binary file
 */
void
check_file_error(FILE *file){
	if(feof(file)){
		//TODO
		//printf("Reached end of file\n");
	}
	else if(ferror(file)){
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
	

	for(int i = 0; i < MEM_SIZE; i++){
		if(fread(memory->byte + i, BYTES, 1, file) == 1){
			//printf("Line 222 in mem_proc.c\n");
			continue;
		}
		else{
			check_file_error(file);
			break;
		}

	}

	cpu_cycle();

	//Possible deallocation of memory here

//	memory_machine_destroy(&memory);
//	memory_cpu_destroy();
//	memory_instr_data_proc_destroy();
//	memory_instr_mult_destroy();
//	memory_instr_single_data_transfer_destroy();
//	memory_instr_branch_destroy();

	fclose(file);
}
