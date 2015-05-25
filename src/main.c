#include "common_instances.h"
#include "memory_proc.h"


/**
 * Checks the number of arguments
 * @param argc Number of command line arguments
 * @return EXIT_SUCCESS if all ok 
 */
int
argc_check(int argc){
	if(argc == 1){
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
	FILE *binFile = fopen(argv[1], "r");

	if(binFile){
		return EXIT_SUCCESS;
	}
	else{
		/* Pointer is NULL */
		return EXIT_FAILURE;
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

	if(!argc_check(argc) || !argv_check(argv)){
		return EXIT_FAILURE;
	}
	else{
		load_file(argv[1]);	
	}
		return EXIT_SUCCESS;

	return EXIT_SUCCESS;
}
