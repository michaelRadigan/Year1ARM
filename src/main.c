#include "common_instances.h"

/**
 * Checks the number of arguments
 * @param argc Number of command line arguments
 * @return EXIT_SUCCESS if all ok 
 */
int
argc_check(int argc){
	
	if(argc == 2){
		return EXIT_SUCCESS;
	}
	else{
		return EXIT_FAILURE;
	}
}


/**
 * Checks whether a binary file is passed as an argument
 * @param argv Array of command line arguments
 * @return EXIT_SUCCESS if all ok
 */
int
argv_check(char **argv){
	FILE *binFile = fopen(argv[1], "rb");

	if(binFile == NULL){
		return EXIT_FAILURE;
	}
	else{
		return EXIT_SUCCESS;
	}
}


/**
 * Application entry point
 * @param argc Number of command line arguments
 * @param argv Array of command line arguments
 * @return EXIT_SUCCESS if all ok
 */
int
main(int argc, char **argv){

	if(argc_check(argc) || argv_check(argv)){
		return EXIT_FAILURE;
	}
	else{
		FILE *binFile = fopen(argv[1], "rb");
		memory_load_file(binFile);
                memory_cpu_init();
                while(memory != 0x0){
                  cpu_cycle();
               }
               print_registers();
	}
	return EXIT_SUCCESS;
}
