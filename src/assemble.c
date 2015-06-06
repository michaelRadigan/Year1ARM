#include "assembler/assemble.h"

/* Structures

If getting from dictionary, remember to cast back to the right type afterwards

DICTIONARY *label_address := (char *str , uint32_t *location);
DICTIONARY *opcode_function := (char *code , (*ptr_func) (char *source));

*/

DICTIONARY *setUPlabel_address(void){
  DICTIONARY *d = createDictionary();
  return d;
}


DICTIONARY *setUPopcode_function(void){

  //SetUp structs containing function pointers  
  setUPFuncStructs();

  DICTIONARY *d = createDictionary();

  //Functions for data Processing
  putElem(d,"and", (void *) dp1);
  putElem(d,"eor",(void *) dp1);
  putElem(d,"sub",(void *) dp1);
  putElem(d,"rsb",(void *) dp1);
  putElem(d,"add",(void *) dp1);
  putElem(d,"orr",(void *) dp1);
  putElem(d,"mov",(void *) dp2);
  putElem(d,"tst",(void *) dp3);
  putElem(d,"teq",(void *) dp3);
  putElem(d,"cmp",(void *) dp3);

  //Functions for Multiply
  putElem(d,"mul",(void *) m);
  putElem(d,"mla",(void *) ma);

  //Functions for single data transfer
  putElem(d,"ldr",(void *) ldr);
  putElem(d,"str",(void *) str);

  //Functions for branching
  putElem(d,"beq",(void *) b);
  putElem(d,"bne",(void *) b);
  putElem(d,"bge",(void *) b);
  putElem(d,"blt",(void *) b);
  putElem(d,"bgt",(void *) b);
  putElem(d,"ble",(void *) b);
  putElem(d,"b",(void *) b);

  //Functions for specials
  putElem(d,"andeq",(void *) andeq);
  putElem(d,"lsl",(void *) lsl);

  return d;
}

/* Frees all memory allocated by the function dictionary */
void destroyDictionaryfunctions(DICTIONARY *d){
  destroyFuncStructs();
  destroyDictionary(d);
}

/* Checks if label exists, This will store it*/
int storeLabel(char *source){
//  printf("storeLabel source = %s\n", source);
  char *t1 = malloc(sizeof(source));
  char *t2 = malloc(sizeof(source));
  sscanf(source,"%[^:] %[:] ",t1,t2);
//  printf("storeLabel t1 = %s\n", t1);
 // printf("storeLabel t2 = %s\n", t2);
  if(t2[0] == '\0'){ //No Label
    return 0;
  }
  uint32_t *linenumlabel = malloc(sizeof(uint32_t *));
  *linenumlabel = file_line;
//  uint32_t *num = linenumlabel;
  putElem(label_address,t1,linenumlabel);
//  printf("Printing tree in removelabel....\n");
//  printPaths(label_address->tree);
//  free(t1);
  free(t2);
//  free(linenumlabel);
  return 1;
}

/* Removes the label from the source string if it exists */
char *removeLabel(char *source){
  char *t1 = malloc(sizeof(source));
  char *t2 = malloc(sizeof(source));
  sscanf(source,"%[^:] %*[ :] %[^:] ",t1,t2);
  if(t2[0] == '\0'){
    free(t2);
    return(t1);
  }
  free(t1);
  return(t2);
}

/*prints the bits to file, return 1 on success, 0 otherwise */

/*void writeBits(uint32_t *bits , FILE *out_file){
  assert(out_file != NULL);
  int i;
  uint32_t mask = 1 << 31;

  for(i=0 ; i<32 ; ++i){
    if((*bits & mask) == 0){
      printf("0");
    }else{
      printf("1");
    }
      
    *bits <<= 1;
  }

//  writeUint32(out_file, bits);
  printf("\n");
}
*/

int writeUint32(FILE *const stream, uint32_t value) {
  /* These must be unsigned */
  unsigned char buffer[sizeof(uint32_t)];
  /* Usually 0xFF */
  const unsigned charMask = (1 << CHAR_BIT) - 1;
  for(int i=0; i < sizeof(buffer); ++i) {
    /* Place the MSB first */
    buffer[sizeof(buffer) - i - 1] = value & charMask;
    value >>= CHAR_BIT;
  }
  int count = fwrite(buffer,sizeof(buffer),1, stream);
  return count == 1;
}


/* Checks for file existance */
int doesFileExist(const char *filename) { 
  struct stat st;
  int result = stat(filename, &st);
  return result == 0;

}


/* Prints the content of a file*/
int printFileContents(FILE *ptr){
  printf("\nThe File Contents are:\n");
  int c;
  while(1)
  {
    c = fgetc(ptr);
    if( feof(ptr) )
      { 
        break ;
      }
    printf("%c", c);
  }
  printf("\n");
  rewind(ptr);
  return 0;
}


uint32_t LEtoBE(uint32_t word){
  word = ((word << 8) & 0xFF00FF00) | ((word >> 8) & 0x00FF00FF);
  word = (word << 16) | (word >> 16);
  return word;
}

/* MAIN Program loop */
int main(int argc, char **argv) {
  assert(argc == 3);

  const int MAX_LINE_LENGTH = 511;
//  printf("Program name %s\n", argv[0]);
//  printf("arg[1]: %s\n", argv[1]);
//  printf("arg[2]: %s\n", argv[2]);

  //Setup Dictionaries
  label_address = setUPlabel_address();
  setUPcode_binarycode();
  opcode_function = setUPopcode_function();
  printf("Value of b %x\n", *(uint32_t *)getElem(code_binarycode, "b"));
//  printf("Printing binarycode tree in main....\n");
//  printPaths(code_binarycode->tree);


  //Setup File fields
  FILE *ptr_SourceFile = NULL;
  FILE *ptr_WriteFile = NULL;


  char buff[MAX_LINE_LENGTH];

  ptr_SourceFile = fopen( argv[1] ,"r");

  if(ptr_SourceFile == NULL || !doesFileExist(argv[1])){
    printf("SOURCEFILE NON EXISTANT\n");
    return EXIT_FAILURE;
  }

  printFileContents(ptr_SourceFile);

  //Will create file if non existant
  ptr_WriteFile = fopen( argv[2] , "w+");


  printf("GOT HERE\n");


  /* Program Loop 1*/
  /* Creates Dictionary for Labels and Memory Locations */

  printf("Program Loop 1\n");
  file_line = 0;

  while(fgets(buff,MAX_LINE_LENGTH,ptr_SourceFile)){
//	    printf("Program loop 1 buffer line %x = %s\n", file_line ,buff);
    storeLabel(buff);
    file_line++;
  }
//  printf("Printing tree....\n");
//  printPaths(label_address->tree);

  printf("FInished program loop 1\n");

  rewind(ptr_SourceFile);
  file_line = 0;

  printf("Beginning Program loop2\n");
  /* Program Loop 2*/
  /* Reads Opcode and generate Binary Encoding */
  while(fgets(buff,MAX_LINE_LENGTH,ptr_SourceFile)){

    //Check if label exists and if so remove it
    char *buffer = removeLabel(buff);
    //Buff now contains no label absolutely

    //Duplicate Buffer
    char *buffTemp;
    if((buffTemp = malloc(sizeof(buffer)) ) == NULL){
      printf("Problem! couldn't allocate memory for buffTemp");
      break;
    }
    strcpy(buffTemp,buff);
    printf("This is your buffer = %s\n", buff);
//    uint32_t *output;
    printf("Program loop 2 buffTemp = %s\n", buffTemp);
    const char s[2] = " ";
    char *token = strtok(buff,s);
    printf("current token %s\n" , token);
    //Check if empty line
    if(token[0]=='\n'){
      continue;
    }
    if (token[strlen(token)-2] == ':') {
    	printf("This is a label: %s\n", token);
        free(buffTemp);
        file_line++;
        continue;
    }
    //Loop-up Opcode to get function
    STR_ENC *encodingStruct;

    if((encodingStruct = (STR_ENC *)getElem(opcode_function , (void *)token)) == NULL){
      printf("FAILURE FROM DICTIONARY, OPCODE DOES NOT EXIST: %s" , token);
      exit(EXIT_FAILURE);
    }

    //Apply function
//	printf("buffTemp loop 2 = %s\n", buffTemp);
    printf("This is buffTemp %s\n", buffTemp);
 //   output = encodingStruct->encFunc(buffTemp);
    uint32_t out = *encodingStruct->encFunc(buffTemp);

    out = LEtoBE(out);
    printf("hex = %x\n", out);
    printf("hex in the middle = %x\n", out);
    //Write to file
    writeUint32(ptr_WriteFile, out);
    printf("hex again  = %x\n", out);
    free(buffTemp);
    file_line++;
  }
  printf("file_line = %x\n", file_line);

  printf("Finished Program Loop 2\n");

  fclose(ptr_SourceFile);
  fclose(ptr_WriteFile);

  destroyDictionary(label_address);
  destroycode_binarycode();
  destroyDictionaryfunctions(opcode_function);


   return EXIT_SUCCESS;
  }
