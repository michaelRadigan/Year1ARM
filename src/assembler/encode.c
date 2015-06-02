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

uint32_t *toCpuReg(char *str){
  //only the beginning R will ever be lower/uppercase
  str[0] = toupper(str[0]);
  return (cpu_reg *)str;
}


/* char *source is the string of words of the instructions */

/*There is a lot of redundancy her that we could proably clean up*/

/* Translates and, eor, sub, rsb, add ,orr instructions*/
uint32_t *dataProcessing1(char *source){
     assert(!= NULL);
    uint32_t first12bits;
    char *rn;
    char *rd
    uint32_t rotAndImm;
    char *operand2;

    const char delim[3] = " ,";
    char *opcode = strtok(source, delim);
    if(opcode == NULL){
        printf("OPCODE PARAMETER NON-EXISTANT");
        exit(EXIT_FAILURE);
    }
    /*The 2nd letter of each mnemonic is unique, so we will just test that*/
    if(opcode[1] == 'n'){
        first12bits = 0xE2000000; // This will have to be changed if we decide 
                                  // decide to implement th eoptional shifts
    } else if(opcode[1] == 'o'){
        first12bits = 0xE2200000; // ^^^^^^^^
    } else if(opcode[1] == 'u'){
        first12bits = 0xE2400000; // ^^^^^^^^
    } else if(opcode[1] == 's'){
        first12bits = 0xE2600000; // ^^^^^^^^
    } else if(opcode[1] == 'd'){
        first12bits = 0xE2800000; // ^^^^^^^^
    } else if(opcode[1] == 'r'){
        first12bits = 0xE3800000; // ^^^^^^^^
    }
    if((rn = strtok(NULL, delim)) == NULL){
        printf("OPCODE PARAMETER NON-EXISTANT");
        exit(EXIT_FAILURE);
    }
    if((rd = strtok(NULL, delim)) == NULL){
        printf("OPCODE PARAMETER NON-EXISTANT");
        exit(EXIT_FAILURE);
    }

    
    if (operand2 = strtok(NULL, delim) == NULL){
        printf("OPCODE PARAMETER NON-EXISTANT");
        exit(EXIT_FAILURE);
    }
    if(operand2[0] == '#'){
        rotAndImm = convertToImm(sscanf(operand[1], "%" SCNx32, &int32));
    } else {
        /*this is where we could put the optional shift stuff*/
    }
    uint32_t *rnInt = (uint32_t *) toCpuReg(rn);
    uint32_t *rdINt = (uint32_t *) toCpuReg(rd);
    return (first12bits | (*rnInt) << 16 | (*rdInt) << 12 | rotAndImm);
}

    
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


/*Takes in a 32 bit unisgned vlaue and returns a 32 bit unsigned int
 *which represents the rotate-Immediate value representation of the input as
 *descried on page 7 of the spec. (the first 20 bits of the return value will 
 *be 0
 *@Param extractedExp is the value to be converted*/
uint32_t
convertToImm(uint32_t extractdExp){
    /*If the number is less than 2^8 then it's easy*/
    if(extractedExp < 2^8){
        return extractedExp;
    }
    /*Check condition 1: There must be atleast 24 0s in a row*/
    int numOFZeroes = numberOfZeroes(extractedExp);
    if(numOFZeroes < 24){
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
    int lsb = least_significant_bit(extractedExp);
    if(msb > 7){ 
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
           if(posLastOne % 2 == 1){
           /*the rotation is an odd number, so check if the final bit on the
            *hand-side is a 0, if it isn't then it can'tbe represented.*/
               if(((extractedExp >> (32 - (7 - posLastOne))) & 0x1) == 0){
                   rotate = (7 - posLastOne - 1)/2;
                   imm = execute_rotate_right((32 - posLastOne), extractedExp);
               } else {
               /*Throw error, this number can't be represented in this way*/
               }
           }
           rotate = (7 - posLastOne)/2;
           imm = execute_rotate_right((31 - posLastOne), extractedExp);
        }
    } else { 
        /*this case should have been covered at the start, if it reaches here
         *then throw an error*/
    }
    return imm | (rotate << 8);
     
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
        uint32_t extractedExp = sscanf(operand2[1], "%" ,SCNx32, &int32);
        /*Now need to convert to from described in emulate*/

        uint32_t rotAndImm = convertToImm(extractedExp);
        uint32_t sameForAll = 0xE3A00000;
        uint32_t *regint = (uint32_t *) toCpuReg(reg);
        return (sameForAll | (*regint << 12) | rotAndImm);

    } else {
        /*We can include the case for a shifted register here if we choose to*/
    }
  }
}

/* Translates tst, teq, cmp */
uint32_t *dataProcessing3(char *source){
    assert(!= NULL);
    uint32_t first12bits;
    char *rn;
    uint32_t rotAndImm;
    char *operand2;

    const char delim[3] = " ,";
    char *opcode = strtok(source, delim);
    if(opcode == NULL){
        printf("OPCODE PARAMETER NON-EXISTANT");
        exit(EXIT_FAILURE);
    }
    if(opcode[0] == 'c'){
        first12bits = 0xE3500000; // This will have to be changed if we decide 
                                  // decide to implement th eoptional shifts
    } else if(opcode[1] == 's'){
        first12bits = 0xE3100000; // ^^^^^^^^
    } else {
        first12bits = 0xE3300000; // ^^^^^^^^
    } else {
       /*ERROR - passed this function something wrong*/
    }    
    if((rn = strtok(NULL, delim)) == NULL){
        printf("OPCODE PARAMETER NON-EXISTANT");
        exit(EXIT_FAILURE);
    }
    
    if (operand2 = strtok(NULL, delim) == NULL){
        printf("OPCODE PARAMETER NON-EXISTANT");
        exit(EXIT_FAILURE);
    }
    if(operand2[0] == '#'){
        rotAndImm = convertToImm(sscanf(operand[1], "%" SCNx32, &int32));
    } else {
        /*this is where we could put the optional shift stuff*/
    }
    uint32_t *rnInt = (uint32_t *) toCpuReg(rn);
    return (first12bits | (*rnInt) << 16 | rotAndImm);
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

