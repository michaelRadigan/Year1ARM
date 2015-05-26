#include "common_instances.h"

#define MEM_SIZE 65536
#define BYTES    4


memory_machine *memory;


/**
 * Performs Little to Big endian swapping
 * when reading from Little endian file
 * @param word Little endian word from memory
 * @return word in Big endian ready for processing
 */
uint32_t
memory_fetch_word_le_to_be(uint32_t word){
    uint32_t swapped = ((word >> 24) & 0xff)     | /* move byte 3 to byte 0*/
                   	   ((word >> 8)  & 0xff00)   | /* move byte 2 to byte 1*/
                   	   ((word << 8)  & 0xff0000) | /* move byte 1 to byte 2*/
					   ((word << 24) & 0xff000000); /* byte 0 to byte 3*/
	return swapped;
}


/**
 * Fetches 4 bytes from memory and concatenates in 1 word
 * @param pc Program counter acts as index to memory
 */
uint32_t
memory_fetch_word(uint32_t pc){
    uint8_t byteOne   = memory->byte[pc];
	uint8_t byteTwo   = memory->byte[pc + 1];
	uint8_t byteThree = memory->byte[pc + 2];
	uint8_t byteFour  = memory->byte[pc + 3];

	/* Do bit-wise shifting to concatenate the four bytes
	 * Input is little endian, hence shift like so
	 */
	/*Possible source of bugs here*/
	uint32_t result = byteOne | (byteTwo << 8) | (byteThree << 16) | (byteFour << 24);

	uint32_t word_in_be = memory_fetch_word_le_to_be(result);

	return word_in_be;
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
 * Sets up cpu 
 * @param cpuptr Pointer to cpu
 */
void
memory_cpu_init(struct Cpu *cpuptr){
	/* Need to implement with calloc to initialise to zero*/
}


/**
 * Frees cpu
 * @param cpuptr Pointer to cpu
 */
void
memory_cpu_destroy(struct Cpu *cpuptr){
	free(cpuptr);
}


/**
 * Checks whether EOF or an error has occured
 * @param file Pointer to the binary file
 */
void
check_file_error(FILE *file){
	if(feof(file)){
		printf("Reached end of file\n");
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

    memory_machine *memory = NULL;
	memory_machine_init(memory);
	
	for(int i = 0; i < MEM_SIZE; i++){
		if(fread(&memory->byte[i], BYTES, 1, file) == 1){
	
		printf("here\n");
			continue;
		}
		else{
			check_file_error(file);
			break;
		}

	}
	//Possible deallocation of memory here
	fclose(file);
}

