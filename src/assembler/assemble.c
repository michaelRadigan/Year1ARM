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
  DICTIONARY *d = createDictionary();

  //Functions for data Processing
  putElem(d,"and",(void *) dataProcessing1);
  putElem(d,"eor",(void *) dataProcessing1);
  putElem(d,"sub",(void *) dataProcessing1);
  putElem(d,"rsb",(void *) dataProcessing1);
  putElem(d,"add",(void *) dataProcessing1);
  putElem(d,"orr",(void *) dataProcessing1);
  putElem(d,"mov",(void *) dataProcessing2);
  putElem(d,"tst",(void *) dataProcessing3);
  putElem(d,"teq",(void *) dataProcessing3);
  putElem(d,"cmp",(void *) dataProcessing3);

  //Functions for Multiply
  putElem(d,"mul",(void *) multiply);
  putElem(d,"mla",(void *) multiplyAccum);

  //Functions for single data transfer
  putElem(d,"ldr",(void *) sdt_ldr);
  putElem(d,"str",(void *) sdt_str);

  //Functions for branching
  putElem(d,"beq",(void *) branch);
  putElem(d,"bne",(void *) branch);
  putElem(d,"bge",(void *) branch);
  putElem(d,"blt",(void *) branch);
  putElem(d,"bgt",(void *) branch);
  putElem(d,"ble",(void *) branch);
  putElem(d,"b",(void *) branch);

  //Functions for specials
  putElem(d,"andeq",(void *) spec_andeq);
  putElem(d,"lsl",(void *) spec_lsl);

  return d;
}

/* Note: This function returns a pointer to a substring of the original string.
 If the given string was allocated dynamically, the caller must not overwrite
 that pointer with the returned value, since the original pointer must be
 deallocated using the same allocator with which it was allocated.  The return
 value must NOT be deallocated using free() etc.*/
char *trimwhitespace(char *str)
{
  char *end;

  // Trim leading space
  while(isspace(*str)) str++;
  if(*str == 0)  // All spaces?
    return str;
  
   // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace(*end)) end--;

     // Write new null terminator
    *(end+1) = 0;

  return str;
}

/* Returns label if label exists on current line else returns NULL */
char *getLabel(char *source){
  char s[2] = ":";
  return strtok(source,s);
}

int main(int argc, char **argv) {
  assert(argc == 2);

  const int MAX_LINE_LENGTH = 511;
  

  //Setup Dictionaries
  label_address = setUPlabel_address;
  code_binarycode = setUPcode_binarycode;
  opcode_function = setUPopcode_function;

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
    
    uint32_t output; 
    const char s[2] = " ";  
    char *token;
    
    if(getLabel(buff)!=NULL){
      const char p[2] = ":";
      strtok(buff,p);
    }else{
      buff = trimwhitespace(buff);
    }

    if((token = strtok(buff,s))==NULL){
      continue;
    }

    uint32_t (*func_ptr)(char *);
    // not sure if should cast to (uint32_t *) or not
    if((func_ptr = (uint32_t) getElem(opcode_function , (void *)token)) == NULL){
      printf("GET FAILURE FROM DICTIONARY, OPCODE DOES NOT EXIST: %s" , token);
    }
    
    output = func_ptr(buff);

    //will return negative number if failed
    if((fprintf(ptr_WriteFile , "%x" , output)) < 0){
      printf("WRITE FAILURE on %x" , output);
      break;
    }
  }



  fclose(ptr_SourceFile);
  fclose(ptr_WriteFile);
  
  destroyDictionary(label_address);
  destroyDictionary(code_binarycode);
  destroyDictionary(opcode_function);


  return EXIT_SUCCESS;
}
