#include "assemble.h"


/* Structures

If getting from dictionary, remember to cast back to the right type afterwards

DICTIONARY *label_address := (char *str , uint32_t *location);
DICTIONARY *code_binarycode := (char *code , uint32_t binaryVal);
DICTIONARY *opcode_function := (char *code , (*ptr_func) (char *source));

*/



DICTIONARY *setUPlabel_address(void){
  DICTIONARY *d = createDictionary();
  return d;
}

DICTIONARY *setUPcode_binarycode(void){
  DICTIONARY *d = createDictionary();

  //Opcodes for binaryConditions
  putElem(d,"eq",(void *) 0x0);
  putElem(d,"ne",(void *)0x1);
  putElem(d,"ge",(void *)0xa);
  putElem(d,"lt",(void *)0xb);
  putElem(d,"gt",(void *)0xc);
  putElem(d,"le",(void *)0xd);
  putElem(d,"al",(void *)0xe);

  //Opcodes for Data Processing
  putElem(d,"and",(void *)0x0);
  putElem(d,"eor",(void *)0x1);
  putElem(d,"sub",(void *)0x2);
  putElem(d,"rsb",(void *)0x3);
  putElem(d,"add",(void *)0x4);
  putElem(d,"orr",(void *)0xc);
  putElem(d,"mov",(void *)0xd);
  putElem(d,"tst",(void *)0x8);
  putElem(d,"teq",(void *)0x9);
  putElem(d,"cmp",(void *)0xa);
   
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

void destroyDictionaryfunctions(DICTIONARY *d){
  destroyFuncStructs();
  destroyDictionary(d);
}


/* Returns label if label exists on current line else returns NULL */
char *getLabel(char *source){
  char s[2] = ":";
  return strtok(source,s);
}


/*prints the bits to file, return 1 on success, 0 otherwise */
void writeBits(uint32_t *bits , FILE *out_file){
  assert(out_file != NULL);
  int i;
  uint32_t mask = 1 << 31;

  for(i=0 ; i<32 ; ++i){
    if((*bits & mask) == 0){
      fprintf(out_file , "0");
    }else{
      fprintf(out_file , "1");
    }
      
    *bits <<= 1;
  }
  fprintf(out_file , "\n");
}


/* MAIN Program loop */

int main(int argc, char **argv) {
  assert(argc == 2);

  const int MAX_LINE_LENGTH = 511;
  

  //Setup Dictionaries
  label_address = setUPlabel_address();
  code_binarycode = setUPcode_binarycode();
  opcode_function = setUPopcode_function();
  

  //Setup File fields
  FILE *ptr_SourceFile;
  FILE *ptr_WriteFile;


  char buff[MAX_LINE_LENGTH];

  

  ptr_SourceFile = fopen( argv[0] ,"r");

  if (!ptr_SourceFile){  
    return EXIT_FAILURE;
  }

  ptr_WriteFile = fopen( argv[1] , "w+");

  if(!ptr_WriteFile){
    return EXIT_FAILURE;
  }


  /* Program Loop 1*/
  /* Creates Dictionary for Labels and Memory Locations */
  while (fgets(buff,MAX_LINE_LENGTH, ptr_SourceFile)!=NULL){
         
    char *label;

    if((label = getLabel(buff)) !=NULL){
      putElem(label_address , label , (void *) &label);
    }

  }

  rewind(ptr_SourceFile);

  /* Program Loop 2*/
  /* Reads Opcode and generate Binary Encoding */
  while (fgets(buff,MAX_LINE_LENGTH, ptr_SourceFile)!=NULL){
    
    uint32_t *output; 
    const char s[2] = " ";  
    char *token;
    
    if(getLabel(buff)!=NULL){
      const char p[2] = ":";
      strtok(buff,p);
    }

    if((token = strtok(buff,s))==NULL){
      continue;
    }

    STR_ENC *encodingStruct;
    if((encodingStruct = (STR_ENC *)getElem(opcode_function , (void *)token)) == NULL){
      printf("FAILURE FROM DICTIONARY, OPCODE DOES NOT EXIST: %s" , token);
    }
    
    output = encodingStruct->encFunc(buff);

    writeBits(output, ptr_WriteFile); 
  }


  fclose(ptr_SourceFile);
  fclose(ptr_WriteFile);
  
  destroyDictionary(label_address);
  destroyDictionary(code_binarycode);
  destroyDictionaryfunctions(opcode_function);


  return EXIT_SUCCESS;
}
