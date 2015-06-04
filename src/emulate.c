#include "emulator/common_instances.h"


/**
 * Checks the number of arguments
 * @param argc Number of command line arguments
 */
int
argc_check(int argc){	
  return argc == 2;
}


/**
 * Checks whether a binary file is passed as an argument
 * @param argv Array of command line arguments
 * @return EXIT_SUCCESS if all ok
 */
int
argv_check(char **argv){

  FILE *binFile = fopen(argv[1], "rb");
  return binFile == NULL;
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
