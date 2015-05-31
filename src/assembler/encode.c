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
  putElem(d,"eq",(void *)0x0);
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

  code_binarycode = d;
}

void destroycode_binarycode(void){
  destroyDictionary(code_binarycode);
}


/*  
* Concatenates 2 binary encodings from right to left
* @param b1: the bits to be concatentated onto the left of b2 at bit position pos
* @param param: if 1 then replacement operation else, original concat operation
* @param numberOfBits: the number of significant bits in b1
* @param b2: the original bits to be concatenated onto.
* @param pos: the number of bits from the right at which b1 is to begin on b2
*
* @return: uint32_t bitString with b1 from pos onwards on b2
*
* Note: Always build bit strings from the right to left
*/
uint32_t *binaryConcat(int param , uint32_t *b1 ,int numberOfBits, 
                          uint32_t *b2 , int pos){
  //PRE: After pos in b2, there exist only 0 bits.
  if(param == 1){
    //replaces the bit of b2 to be replaced with 0s
    uint32_t mask = 1;
    for(int i = 0 ; i <  numberOfBits; i++){
      mask = 1 + (mask << 1); // put 1s in correct place
    }
    b2 =  *b2 & ((~mask) << pos); // and so 0s are in swap position
  }
  return *b2 | (*b1 << pos);
}

cpu_reg *toCpuReg(char *str){
  //only the beginning R will ever be lower/uppercase
  str[0] = toUpper(str[0]);
  return (cpu_reg *)str
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
  uint32_t *binary;

  const char p[2] = " ,";
  strtok(source , p);
 
  //Parse source string
  char *rd;
  char *rm;
  char *rs;

  if((rd = strtok(NULL , p)) == NULL){
    printf("OPCODE PARAMETER NONEXISTANT")
    exit(EXIT_FAILURE);
  }
  if((rm = strtok(NULL , p)) == NULL){
    printf("OPCODE PARAMETER NONEXISTANT")
    exit(EXIT_FAILURE);
  }
  if((rs = strtok(NULL , p)) == NULL){
    printf("OPCODE PARAMETER NONEXISTANT")
    exit(EXIT_FAILURE);
  }

  //Create uint32_t binary format
  uint32_t *rdb = (uint32_t *) toCpuReg(rd);
  uint32_t *rmb = (uint32_t *) toCpuReg(rm);
  uint32_t *rsb = (uint32_t *) toCpuReg(rs);
  uint32_t *s = 0x0;
  uint32_t *a = 0x0;
  uint32_t *cond = 0xE;

  binary = binaryConcat(0, &0x9 , 4 , rmb , 4);
  binary = binaryConcat(0, rs , 4 , binary , 8);
  binary = binaryConcat(0,rdb 4, binary , 16);
  binary = binaryConcat(0,s,1, binary , 20);
  binary = binaryConcat(0,a,1, binary , 21);
  binary = binaryConcat(0,cond 4, binary , 28);

  return binary;
}

/* Translates mla */
uint32_t *multiplyAccum(char *source){
    
  uint32_t binary = multiply(source);
  char *rn;

  if((rn = strtok(NULL, " ,")==NULL)){
    printf("OPCODE PARAMETER NONEXISTANT")
  }

  uint32_t *rnb = (uint32_t *) toCpuReg(rn);
  uint32_t *a = 0x1;
  binary = binaryConcat(1,rn,4,binary,12);
  binary = binaryConcat(1,a,1,binary,21);

  return binary;
}

/* Translates ldr */
uint32_t *sdt_ldr(char *source){
  //TODO
  return NULL;
}

/* Translates str */
uint32_t *sdt_str(char *source){
  //TODO
  return NULL;
}

/* Translates branch - beq, bne, bge, blt, bgt, ble, b, */
uint32_t *branch(char *source){
  //TODO
  return NULL;
}

/* Translates special - andeq */
uint32_t *spec_andeq(char *source){
  //TODO
  return NULL;
}

/* Translates special - lsl */
uint32_t *spec_lsl(char *source){
  //TODO
  return NULL;
}

