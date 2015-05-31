#include "common_instances.h"

/**
 * Checks the number of arguments
 * @param argc Number of command line arguments
 * @return EXIT_SUCCESS if all ok 
 */
int
argc_check(int argc){
	
	if(argc == 2){
		return 1;
	}
	else{
		return 0;
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
		return 0;
	}
	else{
		return 1;
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
		FILE *binFile = fopen(argv[1], "rb");
		memory_load_file(binFile);
	}
	else{
		return EXIT_FAILURE;

	}
	return EXIT_SUCCESS;
}
