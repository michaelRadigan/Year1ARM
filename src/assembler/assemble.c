#include "assemble.h"


/* Structures

DICTIONARY *label_address;
DICTIONARY *code_binarycode;
DICTIONARY *opcode_function;

*/



DICTIONARY *setUPlabel_address(void){
  //TODO
  return NULL;
}

DICTIONARY *setUPcode_binarycode(void){
  //TODO
  return NULL;
}

DICTIONARY *setUPopcode_function(void){
  //TODO
  return NULL;
}

int main(int argc, char **argv) {
  assert(argc == 2);

  const int MAX_LINE_LENGTH = 511;
  
    
  

  FILE *ptr_SourceFile;
  FILE *ptr_WriteFile;


  char buf[MAX_LINE_LENGTH];

  

  ptr_SourceFile = fopen( argv[0] ,"r");

  if (!ptr_SourceFile){  
    return EXIT_FAILURE;
  }

  ptr_WriteFile = fopen( argv[1] , "+w");

  if(!ptr_WriteFile){
    return EXIT_FAILURE;
  }


  /* Program Loop 1*/
  /* Creates Dictionary for Labels and Memory Locations */
  while (fgets(buf,MAX_LINE_LENGTH, ptr_SourceFile)!=NULL){
      
    printf("%s",buf);
   
 


  }

  rewind(ptr_SourceFile);

  /* Program Loop 2*/
  /* Reads Opcode and generate Binary Encoding */
  while (fgets(buf,MAX_LINE_LENGTH, ptr_SourceFile)!=NULL){
      
    printf("%s",buf);
   



  }



  fclose(ptr_SourceFile);
  fclose(ptr_WriteFile);

  
  return EXIT_SUCCESS;
}
