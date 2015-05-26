#include "common_instances.h"

#define MEM_SIZE 65536
#define BYTES    4


/**
 * Sets up machine memory
 * @param memptr Pointer to machine memory
 */
void
memory_machine_init(memory_machine *memptr){

	int numBytes = MEM_SIZE;

	memptr = calloc(sizeof(memory_machine)); 
	memptr->byte = calloc(numBytes * sizeof(uint8_t));
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
memory_cpu_init(cpu *cpuptr){
	/* Need to implement with calloc to initialise to zero*/
}


/**
 * Frees cpu
 * @param cpuptr Pointer to cpu
 */
void
memory_cpu_destroy(cpu *cpuptr){
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

    memory_machine *memptr = NULL;
	memory_machine_init(memptr);
	
	for(int i = 0; i < MEM_SIZE; i++){
		if(fread(&memptr->byte[i], BYTES, 1, file) == 1){
	
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

