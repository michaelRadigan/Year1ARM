#include "encode.h"

void printBits(uint32_t x) {
  int i;
  uint32_t mask = 1 << 31;
  for(i=0; i<32; ++i) {
    if (i%4 == 0 && i!=0) {
      printf(", ");
    }
    if((x & mask) == 0){
      printf("0");
    }
    else {
      printf("1");
    }
    x = x << 1;
  }
  printf("\n");
}

// extracted uint32_t from string in form "#2342" or "#0x23EF8"
uint32_t extractNum(char *num) {
  uint32_t res;
  if (num[2] == 'x') {
    res = strtol(num+3, NULL, 16);
  } else {
    res = atoi(num+1);
  }
  return res;
}

/* 
  Sets up structs containing function pointers 
  This is a stupid method but cannot think of
  anything better at the moment 
*/
void setUPFuncStructs(void){
  dp1 = malloc(sizeof(STR_ENC));
  dp1->encFunc = &dataProcessing1;
  
  dp2 = malloc(sizeof(STR_ENC));
  dp2->encFunc = &dataProcessing2;

  dp3 = malloc(sizeof(STR_ENC));
  dp3->encFunc = &dataProcessing3;

  m = malloc(sizeof(STR_ENC));
  m->encFunc = &multiply;

  ma = malloc(sizeof(STR_ENC));
  ma->encFunc = &multiplyAccum;

  ldr = malloc(sizeof(STR_ENC));
  ldr->encFunc = &sdt_ldr;

  str = malloc(sizeof(STR_ENC));
  str->encFunc = &sdt_str;

  b = malloc(sizeof(STR_ENC));
  b->encFunc = &branch;

  andeq = malloc(sizeof(STR_ENC));
  andeq->encFunc = &spec_andeq;

  lsl = malloc(sizeof(STR_ENC));
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

uint32_t binaryConcatHelper(uint32_t *b1, uint32_t *b2 , int pos) {
	  return(*b2 | (*b1 << pos));
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
//  uint32_t (*ptr)(uint32_t,uint32_t, int) = &binaryConcatHelper;

	uint32_t *ptr = malloc(sizeof(uint32_t *));
	*ptr = binaryConcatHelper(b1, b2, pos);
	return ptr;
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
  uint32_t *num = malloc(sizeof(uint32_t *));
  uint32_t  *res = num;
//  uint32_t  *res;
// return (cpu_reg *)str;
  if (strcmp(str, "R0") == 0) {
	   *res = R0;
  } else if (strcmp(str, "R1") == 0) {
	  *res = R1;
  }else if (strcmp(str, "R2") == 0) {
	  *res = R2;
  }else if (strcmp(str, "R3") == 0) {
	  *res = R3;
  }else if (strcmp(str, "R4") == 0) {
	  *res = R4;
  }else if (strcmp(str, "R5") == 0) {
	  *res = R5;
  }else if (strcmp(str, "R6") == 0) {
	  *res = R6;
  }else if (strcmp(str, "R7") == 0) {
	  *res = R7;
  }else if (strcmp(str, "R8") == 0) {
	  *res = R8;
  }else if (strcmp(str, "R9") == 0) {
	  *res = R9;
  }else if (strcmp(str, "R10") == 0) {
	  *res = R10;
  }else if (strcmp(str, "R11") == 0) {
	  *res = R11;
  }else if (strcmp(str, "R12") == 0) {
	  *res = R12;
  }else if (strcmp(str, "PC") == 0) {
	  *res = PC;
  }else if (strcmp(str, "CPSR") == 0) {
	  *res = CPSR;
  }
//free(num);
return res;
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

/*This function returns 1 if the function has 24(or more) consecutive 0s 
 *(including wrap around) and returns 0 otherwise.*/
int
numberOfZeros(uint32_t extractedExp){
    uint32_t mask = 0x00FFFFFFFF; 
    int i;
    for(i = 0; i < 32; i++){
        if((mask & extractedExp) == 0){
            return 1;
        }         
    }
    return 0;
}


uint32_t
execute_rotate_left(uint32_t shift_amount, uint32_t reg_val){
    return((reg_val << shift_amount) | (reg_val >> (32 - shift_amount)));
}


/*Takes in a 32 bit unisgned vlaue and returns a 32 bit unsigned int
 *which represents the rotate-Immediate value representation of the input as
 *descried on page 7 of the spec. (the first 20 bits of the return value will 
 *be 0
 *@Param extractedExp is the value to be converted*/
uint32_t convertToImm(uint32_t extractedExp){
	printf("extractedExp  = %x\n",extractedExp);
    printBits(extractedExp);
  uint32_t rotated;
  uint32_t i;
  for (i = 0; i < 16; i++) {
    rotated = execute_rotate_left(2*i, extractedExp);
 //   printBits(rotated);
    if (rotated < 256) {
      return  i << 8 | rotated;
    }
  }
  return 0xFFFFFFFF; // this is returned if number is irepresentable 
}

/* char *source is the string of words of the instructions */

/*There is a lot of redundancy her that we could proably clean up*/

/* Translates and, eor, sub, rsb, add ,orr instructions*/
uint32_t *dataProcessing1(char *source){
    assert(source!= NULL);
    uint32_t *first12bits = NULL;
    char *rn;
    char *rd;
    uint32_t finalOperand2 = 0;
    char *operand2;
    const char delim[3] = " ,";
    char *opcode = strtok(source, delim);
    if(opcode == NULL){
        printf("OPCODE PARAMETER NON-EXISTANT");
        exit(EXIT_FAILURE);
    }
    
    /* Convert opcode to binary */
    if((first12bits =(uint32_t *) getElem(code_binarycode,opcode)) == NULL){
      printf("OPCODE IS NOT OF THE RIGHT FORMAT\n");
      exit(EXIT_FAILURE);
    }
    /* Create first 12 bits */
    *first12bits = (*first12bits << 21) | 0xE0000000;
    
    /* Get registers */
    if((rn = strtok(NULL, delim)) == NULL){
        printf("OPCODE PARAMETER NON-EXISTANT");
        exit(EXIT_FAILURE);
    }
    if((rd = strtok(NULL, delim)) == NULL){
        printf("OPCODE PARAMETER NON-EXISTANT");
        exit(EXIT_FAILURE);
    }

    /* Get operand2 */
    if ((operand2 = strtok(NULL, delim)) == NULL){
        printf("OPCODE PARAMETER NON-EXISTANT");
        exit(EXIT_FAILURE);
    }

    /* Check if operand 2 is an immediateValue */
    if(operand2[0] == '#'){
        uint32_t *temp = malloc(sizeof(uint32_t *));
//        sscanf(operand2, "#%x", temp);
        *temp = extractNum(operand2);
        printf("temp val = %x\n", *temp);
        uint32_t k = 0x1;
        first12bits = binaryReplace(&k,1,first12bits,25);
        finalOperand2= convertToImm(*temp);
        free(temp);
    } 
    /* Else It is a shift */
    else {
      //THIS DOESNT ACTUALLY DO THE SHIFT!!
    	if (operand2[0] == 'r') {
        
          uint32_t *temp = malloc(sizeof(char *));
          // sscanf(operand2, "%s\n", temp);
    		  temp = (uint32_t *) toCpuReg(operand2);
    	    finalOperand2 = *temp;
          free(temp);
   	}

    }
    /* Finishing off */
    uint32_t *rnInt = (uint32_t *) toCpuReg(rn);
    printf("rnInt = %x\n", *rnInt);
    uint32_t *rdInt = (uint32_t *) toCpuReg(rd);
    printf("rdInt = %x\n", *rdInt);
    printf("finalOperand2 = %x\n", finalOperand2);
    *rdInt = (*first12bits | (*rdInt) << 16 | (*rnInt) << 12 | finalOperand2);
    return rdInt;
}

/* Translates mov */
uint32_t *dataProcessing2(char *source){

    assert(source != NULL);
    const char delim[3] = " ,";
    source = strtok(source, delim);

    char *reg;
    if((reg = strtok(NULL,delim)) == NULL){
        printf("OPCODE PARAMETER NON-EXISTANT");
        exit(EXIT_FAILURE);
    }

    char *operand2;
    if((operand2 = strtok(NULL, delim))==NULL){
      printf("OPCODE PARAMETER NON_EXISTANT");
      exit(EXIT_FAILURE);
    }
    uint32_t startingBits = 0;
    uint32_t *operandBits = malloc(sizeof(uint32_t *));

    /*May want to add the optional shift here*/
    if (operand2[0] == '#'){ //has the form #expression 
        startingBits = 0xE3A00000;
        
        uint32_t *temp = malloc(sizeof(uint32_t *));
        //sscanf(operand2, "#%x" ,temp);
        *temp = extractNum(operand2);
        printf("temp = %x\n", *temp);

        /*Now need to convert to from described in emulate*/
        *operandBits = convertToImm(*temp);
        free(temp);

    } else {
        /*We can include the case for a shifted register here if we choose tio*/
        
      char *temp = malloc(sizeof(char *));
      sscanf(operand2, "%s\n", temp);
     	operandBits = (uint32_t *) toCpuReg(temp);
      free(temp);
        
      startingBits = 0xE1A00000;

    }
    uint32_t *regint = (uint32_t *) toCpuReg(reg);
    *regint = (startingBits | *regint << 12 | *operandBits);
    free(operandBits);
    return regint;
}



/* Translates tst, teq, cmp */
uint32_t *dataProcessing3(char *source){
    assert(source != NULL);
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
    } else if(opcode[1] == 'e'){
        first12bits = 0xE3300000; // ^^^^^^^^
    } else {
       /*ERROR - passed this function something wrong*/
    }    
    if((rn = strtok(NULL, delim)) == NULL){
        printf("OPCODE PARAMETER NON-EXISTANT");
        exit(EXIT_FAILURE);
    }
    
    if ((operand2 = strtok(NULL, delim)) == NULL){
        printf("OPCODE PARAMETER NON-EXISTANT");
        exit(EXIT_FAILURE);
    }
    if(operand2[0] == '#'){
        uint32_t *temp = malloc(sizeof(uint32_t *));
        sscanf(operand2, "#%x", temp);
        rotAndImm = convertToImm(*temp);
        free(temp);
    } else {
        /*this is where we could put the optional shift stuff*/
    }
    uint32_t *rnInt = (uint32_t *) toCpuReg(rn);
    *rnInt =  (first12bits | (*rnInt) << 16 | rotAndImm);
    return rnInt;
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
  char *temp = malloc(sizeof(char *));
  sscanf(rs, "%s\n", temp);
  uint32_t *rsb = (uint32_t *) toCpuReg(temp);
  uint32_t s = 0x0;
  uint32_t a = 0x0;

  uint32_t *cond = malloc(sizeof(uint32_t *));
  *cond = 0xe;

  uint32_t k_ = 0x9;

  binary = binaryConcat(&k_, rmb , 4);
  binary = binaryConcat(rsb , binary , 8);
  binary = binaryConcat(rdb, binary , 16);
  binary = binaryConcat(&s, binary , 20);
  binary = binaryConcat(&a, binary , 21);
  binary = binaryConcat(cond, binary , 28);
  free(temp);
  free(cond);
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
  uint32_t *a = NULL;
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

  char *rd;
  if((rd = strtok( NULL , p)) == NULL){
    printf("OPCODE PARAMETER NONEXISTANT");
    exit(EXIT_FAILURE);
  }
  char *expr;
  if((expr = strtok( NULL , p)) == NULL){
    printf("OPCODE PARAMETER NONEXISTANT");
    exit(EXIT_FAILURE);
  }

  //3 ways of interpreting expr

  //1. Expr is numeric constant of form '=#'
  if(expr[0] == '='){
    uint32_t *val = NULL;
    sscanf(expr , "=%x" , val);
    if(*val < 0xff){
      //effectively a move

    }


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
  char *token = strtok(source, p);
  printf("branch token = %s\n",token);

  uint32_t *binary = NULL;

  uint32_t *cond_b = (uint32_t *)getElem(code_binarycode, token);

  //Get Expression
  uint32_t *labelAddress;

  if((token = strtok(NULL,p)) == NULL){
    printf("OPCODE PARAMETER NONEXISTANT");
    exit(EXIT_FAILURE);
  }
  char  *temp = malloc(sizeof(char *));
  sscanf(token, " %s:", temp);
  //Get Label Value
  if((labelAddress = (uint32_t *)getElem(label_address,temp)) == NULL){
    printf("LABEL NONEXISTANT");
    exit(EXIT_FAILURE);
  }
  free(temp);
  uint32_t *offset = NULL;
  //lines of code offset
  *offset = file_line - *labelAddress;
  *offset += 1;
  //either add 1 or 2 lines

  *binary = (*offset >> 2);
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
  uint32_t base = 0xE1A00000;

  const char p[3] = " ,";
  strtok(source, p);

  char *reg = strtok(NULL , p);
  char *expr = strtok(NULL , p);

  if (reg == NULL) {
    printf("REG PARAMETER NON-EXISTANT\n");
    exit(EXIT_FAILURE);
  }
  uint32_t *regint = toCpuReg(reg);

  uint32_t extractednum = extractNum(expr);
  if (extractednum >= 32) {
    printf("rotation too large\n");
    exit(EXIT_FAILURE);
  } 
  *regint = (base | *regint << 12 | extractednum << 7 | *regint);
  return regint;
}

