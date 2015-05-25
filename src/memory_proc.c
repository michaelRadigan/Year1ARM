#include "common_instances.h"
#include "memory_proc.h"

#define MEM_SIZE 65536
#define BYTES    4



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
	int numBytes = MEM_SIZE;

	memory_machine *machine_mem = malloc(sizeof(memory_machine)); 
	machine_mem->byte = malloc(numBytes * sizeof(uint8_t));

	for(int i = 0; i < MEM_SIZE; i++){
		if(fread(&machine_mem->byte[i], BYTES, 1, file) == 1){
	
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

