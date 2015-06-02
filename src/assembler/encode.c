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
  return *b2 | (*b1 << pos);
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
    b2 =  *b2 & ((~mask) << pos); // and so 0s are in swap position
    return binaryConcat(b1,b2,pos);
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


 /*Takes two ints and returns the larger*/
int
max(int a, int b){
    if(a > b){
        return a;
    } else {
        return b;
    }
    
}


/*Takes in a 64 bit unisgned vlaue and returns a 32 bit unsigned int
 *which represents the rotate-Immediate value representation of the input as
 *descried on page 7 of the spec. (the first 20 bits of the return value will 
 *be 0
 *@Param extractedExp is the value to be converted*/
uint32_t
convertToImm(uint32_t extractdExp){
   /*To test whether thi is possible find th difference between the most 
    *and the least significant bit*/
    if(extractedExp = 0){
        return 0;
    }
    /*Check condition 1: There must be atleast 24 0s in a row*/
    int i, count, max; 
    count = 0;  //The current number f 0s in a row
    max = 0;    //The max number of 0s in a row
    uint32_t mask  = 0x1; 
    for(i = 0; i < 32; i++){
        if((mask & extractedExp) == 0){ //the "i"th bit of extractedExp is 0
            count++;
        } else {
            max = max(max, count);
            count = 0; 
        }
        mask <<= 1;
    }
    if( max < 24){
    /*Throw some kind of error, this number isn't able to be transferred
     *immediate and rotate format*/
    }
    /*There are three scenarios, either the immediate is all together and msb
     *is less than 8, all together and msb > 8 or it is split between
     *from 31 to 0, we can find out which is the case by checking the most 
     *and least significant bits*/
    uint32_t rotate;
    uint32_t imm;
    int msb = most_significant_bit(extractedExp);
    int lsb = east_significant_bit(extractedExp);
    if(msb > 8){ 
        /*check whether split or not*/
        if((msb - lsb) < 8){//it's all together, just need to find the rotation
            /*For the shift to be even the lsb must be even*/
            if(lsb % 2 == 0){
                rotate = (32 - lsb)/2;
                imm = extractedExp >> lsb  ; 
            } else {
                if((msb - lsb) < 7){
                /*The last bit can be 0!*/
                rotate = (33 -lsb)/2; /*rotate right one further*/
                imm = extractedExp >> (lsb - 1);
                } else {
                    /*throw error that it is not possible*/
                }
            }
        } else {
           /*This is the case that it is split*/
           /*Find how much to rotate based on where the last 1 in the right 
             group is*/
           int j;
           int posLastOne = -1;
           uint32_t mask3 = 0x1; //could reuse mask 1?
           for(j = 0; j < 8; j++){
               if((extractedExp & mask3) == 1){
                   posLastOne = j;
               }
           }
           if(posLastOne == -1){
               /*throw error, this can not occur*/
           }
           
           
           

        }


    } else { //We can move this to the start and say if < 2*8...
        return extractedExp;
    }
     
}

/* Translates mov */
uint32_t *dataProcessing2(char *source){
    assert(source != NULL);
    uint32_t binary;
    const char delim[3] = " ,";
    strtok(source, delim);
    char *reg = strtok(NULL, delim);
    if(reg == NULL){
        printf("OPCODE PARAMETER NON-EXISTANT");
        exit(EXIT_FAILURE);

    char *operand2 = strtok(NULL, delim);
    /*May want to add the optional shift here*/
    if (operand2[0] == '#'){ //has the form #expression 
        uint32_t extractedExp = sscanf(operand2[1], "%" SCNx32, &int32);
        /*Now need to convert to from described in emulate*/
        convertToImm(extractedExp);
    }
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

  binary = binaryConcat(&0x9, rmb , 4);
  binary = binaryConcat(rs , binary , 8);
  binary = binaryConcat(rdb, binary , 16);
  binary = binaryConcat(s, binary , 20);
  binary = binaryConcat(a, binary , 21);
  binary = binaryConcat(cond, binary , 28);

  return binary;
}

/* Translates mla */
uint32_t *multiplyAccum(char *source){
  assert(source!=NULL);
  uint32_t binary = multiply(source);
  char *rn;

  if((rn = strtok(NULL, " ,")==NULL)){
    printf("OPCODE PARAMETER NONEXISTANT");
    exit(EXIT_FAILURE);
  }

  uint32_t *rnb = (uint32_t *) toCpuReg(rn);
  uint32_t *a = 0x1;
  binary = binaryReplace(rn,4,binary,12);
  binary = binaryReplace(a,1,binary,21);

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
  assert(source != NULL);
  const char p[3] = " ,";
  char *token;
  token = strtok(source, p);

  uint32_t *binary;

  uint32_t *cond_b = (uint32_t *)getElem(code_binarycode, token);

  //Get Expression
  uint32_t *labelAddress;
  if(token = strtok(NULL,p)==NULL){
    printf("OPCODE PARAMETER NONEXISTANT");
    exit(EXIT_FAILURE);
  }

  //Get Label Value
  if(labelAddress = (uint32_t *)getLabel(label_address) == NULL){
    printf("LABEL NON-EXISTANT");
    exit(EXIT_FAILURE);
  }
  
  uint32_t offset = file_line - *labelAddress;
   
  
  
  
  binary = binaryConcat(0,&0x5,3,binary,25);
  binary = binaryConcat(0,cond_b,4,binary,28);
  
  return binary;
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

