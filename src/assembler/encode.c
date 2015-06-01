#include "encode.h"



/* 
  Sets up structs containing function pointers 
  This is a stupid method but cannot think of
  anything better at the moment 
*/
void setUPFuncStructs(void){
  STR_ENC *dp1 = malloc(sizeof(STR_ENC));
  dp1->encFunc = &dataProcessing1;
  
  STR_ENC *dp2 = malloc(sizeof(STR_ENC));
  dp2->encFunc = &dataProcessing2;

  STR_ENC *dp3 = malloc(sizeof(STR_ENC));
  dp3->encFunc = &dataProcessing3;

  STR_ENC *m = malloc(sizeof(STR_ENC));
  m->encFunc = &multiply;

  STR_ENC *ma = malloc(sizeof(STR_ENC));
  ma->encFunc = &multiplyAccum;

  STR_ENC *ldr = malloc(sizeof(STR_ENC));
  ldr->encFunc = &sdt_ldr;

  STR_ENC *str = malloc(sizeof(STR_ENC));
  str->encFunc = &sdt_str;

  STR_ENC *b = malloc(sizeof(STR_ENC));
  b->encFunc = &branch;

  STR_ENC *andeq = malloc(sizeof(STR_ENC));
  andeq->encFunc = &spec_andeq;

  STR_ENC *lsl = malloc(sizeof(STR_ENC));
  lsl->encFunc = &spec_lsl;

}

void destroyFuncStructs(void){
  free(dp1);
  free(dp2);
  free(dp3);
  free(m);
  free(ma);
  free(ldr);
  free(str);
  free(b);
  free(andeq);
  free(lsl);
}



void setUPcode_binarycode(void){
  DICTIONARY *d = createDictionary();

  //Opcodes for binaryConditions
  putElem(d,"beq",(void *)0x0);//eq
  putElem(d,"bne",(void *)0x1);//ne
  putElem(d,"bge",(void *)0xa);//ge
  putElem(d,"blt",(void *)0xb);//lt
  putElem(d,"bgt",(void *)0xc);//gt
  putElem(d,"ble",(void *)0xd);//le
  putElem(d,"b",(void *)0xe);//al

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

  code_binarycode = d;
}

void destroycode_binarycode(void){
  destroyDictionary(code_binarycode);
}


/*  
* Concatenates 2 binary encodings from right to left
* @param b1: the bits to be concatentated onto the left of b2 at bit position pos
* @param b2: the original bits to be concatenated onto.
* @param pos: the number of bits from the right at which b1 is to begin on b2
*
* @return: uint32_t bitString with b1 from pos onwards on b2
*
* Note: Always build bit strings from the right to left
*/
uint32_t *binaryConcat(uint32_t *b1, uint32_t *b2 , int pos){
  //PRE: After pos in b2, there exist only 0 bits. 
  uint32_t *ret; 
  *ret =  (*b2 | (*b1 << pos));
  return ret;
}



/*  
* Replaces a bit chunk in b2 with b1
* @param b1: the bits to be replaced at pos
* @param numberOfBits: the number of significant bits in b1
* @param b2: the original bits with a chunk to be replaced.
* @param pos: the number of bits from the right at which b1 is to begin on b2
*
* @return: uint32_t bitString with b1 a pos for numberOfBits bits in b2.
*/

uint32_t *binaryReplace(uint32_t *b1, int numberOfBits,uint32_t *b2, int pos){
    //replaces the bit of b2 to be replaced with 0s
  uint32_t mask = 1;
  for(int i = 0 ; i <  numberOfBits; i++){
    mask = 1 + (mask << 1); // put 1s in correct place
  }
  *b2 = *b2 & ((~mask) << pos); // and so 0s are in swap position
  return binaryConcat(b1,b2,pos);
}



/*
* Converts a lowerCase register name to the format 
* specified in cpu_reg of cpu.h (reuse of code) 
* @param str: the name of the register e.g. r2 or r8 
* @return: pointer to the cpu_reg version of the register name.
*/

cpu_reg *toCpuReg(char *str){
  //only the beginning R will ever be lower/uppercase
  str[0] = toupper(str[0]);
  return (cpu_reg *)str;
}


/* char *source is the string of words of the instructions */


/* Translates and, eor, sub, rsb, add ,orr instructions*/
uint32_t *dataProcessing1(char *source){
  //TODO
  return NULL;  
}

/* Translates mov */
uint32_t *dataProcessing2(char *source){
  //TODO
  return NULL;
}

/* Translates tst, teq, cmp */
uint32_t *dataProcessing3(char *source){
  //TODO
  return NULL;
}

/* Translates mul */
uint32_t *multiply(char *source){
  assert(source!=NULL);
  uint32_t *binary;

  const char p[3] = " ,";
  //Remove function name 'mul'
  strtok(source , p);
 
  //Parse source string
  char *rd;
  char *rm;
  char *rs;

  if((rd = strtok(NULL , p)) == NULL){
    printf("OPCODE PARAMETER NONEXISTANT");
    exit(EXIT_FAILURE);
  }
  if((rm = strtok(NULL , p)) == NULL){
    printf("OPCODE PARAMETER NONEXISTANT");
    exit(EXIT_FAILURE);
  }
  if((rs = strtok(NULL , p)) == NULL){
    printf("OPCODE PARAMETER NONEXISTANT");
    exit(EXIT_FAILURE);
  }

  //Create uint32_t binary format
  uint32_t *rdb = (uint32_t *) toCpuReg(rd);
  uint32_t *rmb = (uint32_t *) toCpuReg(rm);
  uint32_t *rsb = (uint32_t *) toCpuReg(rs);
  uint32_t *s = 0x0;
  uint32_t *a = 0x0;

  uint32_t *cond;
  *cond = 0xe;

  uint32_t k_ = 0x9;

  binary = binaryConcat(&k_, rmb , 4);
  binary = binaryConcat(rsb , binary , 8);
  binary = binaryConcat(rdb, binary , 16);
  binary = binaryConcat(s, binary , 20);
  binary = binaryConcat(a, binary , 21);
  binary = binaryConcat(cond, binary , 28);

  return binary;
}

/* Translates mla */
uint32_t *multiplyAccum(char *source){
  assert(source!=NULL);
  uint32_t *binary = multiply(source);
  char *rn;

  if((rn = strtok(NULL, " ,"))==NULL){
    printf("OPCODE PARAMETER NONEXISTANT");
    exit(EXIT_FAILURE);
  }

  uint32_t *rnb = (uint32_t *) toCpuReg(rn);
  uint32_t *a;
  *a = 0x1;

  binary = binaryReplace(rnb,4,binary,12);
  binary = binaryReplace(a,1,binary,21);

  return binary;
}

/* Translates ldr */
uint32_t *sdt_ldr(char *source){
  
  const char p[3] = " ,";
  //Remove function name 'ldr'
  strtok(source , p );

  char *rd = strtok( NULL , p);
  char *expr = strtok(NULL , p);
  
  //3 ways of interpreting expr

  //1. Expr is numeric constant of form '=#'
  if(expr[0] == '='){
    




  }

  //2. A pre-indexed spec

  //3. A post-index
  
  return NULL;
}


/* Translates str */
uint32_t *sdt_str(char *source){
  //Same as ldr but with 20th bit replaced with a 0;
  uint32_t *binary = sdt_ldr(source);
  binary = binaryReplace( 0x0 ,1, binary , 20 );
  return binary;
}


/* Translates branch - beq, bne, bge, blt, bgt, ble, b, */
uint32_t *branch(char *source){
  assert(source != NULL);
  const char p[3] = " ,";
  char *token;
  token = strtok(source, p);

  uint32_t *binary;

  uint32_t *cond_b = (uint32_t *)getElem(code_binarycode, token);

  //Get Expression
  uint32_t *labelAddress;
  if((token = strtok(NULL,p)) == NULL){
    printf("OPCODE PARAMETER NONEXISTANT");
    exit(EXIT_FAILURE);
  }

  //Get Label Value
  if((labelAddress = (uint32_t *)getElem(label_address,token)) == NULL){
    printf("LABEL NONEXISTANT");
    exit(EXIT_FAILURE);
  }
  
  uint32_t *offset;
  *offset = file_line - *labelAddress;
   
  //Calculate offset to add to the binary
  //Must try Understand
  //TODO
  
  binary = offset;
  uint32_t k = 0x5;
  binary = binaryConcat(&k ,binary,25);
  binary = binaryConcat(cond_b,binary,28);
  
  return binary;
}

/* Translates special - andeq */
uint32_t *spec_andeq(char *source){
  //Returns value 0x0
  return 0x0;
}

/* Translates special - lsl */
uint32_t *spec_lsl(char *source){
  //Effectively a 'mov rn,<#expr>' with 'lsl <#expr>' 
  const char p[3] = " ,";
  strtok(source, p);

  char *rn = strtok(NULL , p);
  char *expr = strtok(NULL , p);
  
  //TODO 
  //Problem: translates to 2 different instructions.
  //Possible solution, make FILE objects global, so any method can write to them.
  
  return NULL;
}

