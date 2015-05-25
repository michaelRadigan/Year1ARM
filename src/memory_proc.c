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
		printf("File error end of file\n");
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
<<<<<<< HEAD
memory_load_file(FILE *file){
	int numBytes = MEM_SIZE;

	memory_machine *machine_mem = malloc(sizeof(memory_machine)); 
	machine_mem->word = malloc(numBytes * sizeof(uint8_t));

	for(int i = 0; i < MEM_SIZE; i++){
		if(fread(&machine_mem->word[i], BYTES, 1, file) == 1){
			continue;
		}
		else{
			check_file_error(file);
			break;
		}
	}
	//Possible deallocation of memory here
	fclose(file);
=======
load_file(char *file){
	//do some stuff
        // I don't know who wrote this comment ^^but it was very helpful.
>>>>>>> b69df27eacabde8c6e976d37489af65e143d0917
}

