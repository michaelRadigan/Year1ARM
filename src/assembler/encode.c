#include "encode.h"

void printBits(uint32_t x) {
	int i;
	uint32_t mask = 1 << 31;
	for (i = 0; i < 32; ++i) {
		if (i % 4 == 0 && i != 0) {
			printf(", ");
		}
		if ((x & mask) == 0) {
			printf("0");
		} else {
			printf("1");
		}
		x = x << 1;
	}
	printf("\n");
}

// extracted uint32_t from string in form "#2342" or "#0x23EF8" or "#-1325" or "#-0x3C85"
uint32_t extractNum(char *num) {
	uint32_t res;
	if (num[1] == '-') {
		if (num[3] == 'x') {
			res = -strtol(num + 4, NULL, 16);
		} else {
			res = atoi(num + 1);
		}
	} else {
		if (num[2] == 'x') {
			res = strtol(num + 3, NULL, 16);
		} else {
			res = atoi(num + 1);
		}
	}
	printf("This is the extractedNum %x\n", res);
	return res;
}

int isNegative(uint32_t num) {
	return num >> 31;
}

uint32_t flipSign(uint32_t num) {
	return 0x0 - num;
}


void allocFS(STR_ENC *d , Encode encFunc){
  d = malloc(sizeof(STR_ENC));
  d->encFunc = encFunc;
}

/* 
 Sets up structs containing function pointers
 This is a stupid method but cannot think of
 anything better at the moment
 */
void setUPFuncStructs(void) {

  /* TODO
  allocFS(dp1,&dataProcessing1);
  allocFS(dp2,&dataProcessing2);
  allocFS(dp3,dataProcessing3);
  allocFS(m,multiply);
  allocFS(ma,multiplyAccum);
  
  allocFS(ldr,&sdt_ldr);
  allocFS(str,&sdt_str);
  allocFS(b,&branch);
  allocFS(andeq,&spec_andeq);
  allocFS(lsl,&spec_lsl);
  */

  
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

void destroyFuncStructs(void) {
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

uint32_t *dAlloc(uint32_t val){
  uint32_t *ret;
  if((ret = malloc(sizeof(uint32_t *)))==NULL){
    printf("Trouble allocating memory for dictionary");
    exit(EXIT_FAILURE);
  }
  *ret = val;
  return ret;
}

void setUPcode_binarycode(void) {
	DICTIONARY *d = createDictionary();
//  BR *codes = malloc(sizeof(BR));
	
  //Opcodes for branchConditions
	putElem(d, "beq", dAlloc(0x0));  //eq
	putElem(d, "bne", dAlloc(0x1));  //ne
	putElem(d, "bge", dAlloc(0xa));  //ge
	putElem(d, "blt", dAlloc(0xb));  //lt
	putElem(d, "bgt", dAlloc(0xc));  //gt
	putElem(d, "ble", dAlloc(0xd));  //le
	putElem(d, "b", dAlloc(0xe));  //al

	//Opcodes for Data Processing
	putElem(d, "and", dAlloc(0x0));
	putElem(d, "eor", dAlloc(0x1));
	putElem(d, "sub", dAlloc(0x2));
	putElem(d, "rsb", dAlloc(0x3));
	putElem(d, "add", dAlloc(0x4));
	putElem(d, "orr", dAlloc(0xc));
  putElem(d, "mov", dAlloc(0xd));
	putElem(d, "tst", dAlloc(0x8));
	putElem(d, "teq", dAlloc(0x9));
	putElem(d, "cmp", dAlloc(0xa));

  
  //Opcodes for suffixConditions
  putElem(d , "eq", dAlloc(0x0));
  putElem(d , "ne", dAlloc(0x1));
  putElem(d , "hi", dAlloc(0x8));
  putElem(d , "ls", dAlloc(0x9));
  putElem(d , "ge", dAlloc(0xa));
  putElem(d , "lt", dAlloc(0xb));
  putElem(d , "gt", dAlloc(0xc));
  putElem(d , "le", dAlloc(0xd));
  putElem(d , "al", dAlloc(0xe));

	code_binarycode = d;
}

void setUpLDRconsts(void) {
	LDRconsts = createDictionary();
}

void destroyLDRconsts(void) {
	destroyDictionaryVALUES(LDRconsts);
  //destroyDictionaryKEYS(LDRconsts);
	destroyDictionary(LDRconsts);
}

void setUPregister_dict(void) {
	DICTIONARY *d = createDictionary();
	//Opcodes for registers 
	putElem(d, "r0", dAlloc(0x0));
	putElem(d, "r1", dAlloc(0x1));
	putElem(d, "r2", dAlloc(0x2));
	putElem(d, "r3", dAlloc(0x3));
	putElem(d, "r4", dAlloc(0x4));
	putElem(d, "r5", dAlloc(0x5));
	putElem(d, "r6", dAlloc(0x6));
	putElem(d, "r7", dAlloc(0x7));
	putElem(d, "r8", dAlloc(0x8));
	putElem(d, "r9", dAlloc(0x9));
	putElem(d, "r10", dAlloc(0xa));
	putElem(d, "r11", dAlloc(0xb));
	putElem(d, "r12", dAlloc(0xc));
	putElem(d, "pc", dAlloc(0xf));
	putElem(d, "cpsr", dAlloc(0x10));

	register_dict = d;
}

void destroyRegisterDictionary(void) {
	destroyDictionaryVALUES(register_dict);
	destroyDictionary(register_dict);
}

void destroycode_binarycode(void) {
	destroyDictionaryVALUES(code_binarycode);
	destroyDictionary(code_binarycode);
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

uint32_t binaryReplace(uint32_t b1, int numberOfBits, uint32_t *b2, int pos) {
	//replaces the bit of b2 to be replaced with 0s
	uint32_t mask = 1;
	for (int i = 0; i < numberOfBits - 1; i++) {
		mask = 1 + (mask << 1); // put 1s in correct place
	}
	*b2 = *b2 & ~(mask << pos); // and so 0s are in swap position
	return *b2 | (b1 << pos);
}


/*
 * Converts a lowerCase register name to the format
 * specified in cpu_reg of cpu.h (reuse of code)
 * @param str: the name of the register e.g. r2 or r8
 * @return: pointer to the cpu_reg version of the register name.
 */
uint32_t toCpuReg(char *str) {
	str[0] = toupper(str[0]);
	uint32_t res;
  switch(str[1]){
    case '0': res = R0; break;
    case '1': 
        switch(str[2]){
          case '\0': res = R1; break;
          case '0' : res = R10; break;
          case '1' : res = R11; break;
          case '2' : res = R12; break;           
        }
        break;
    case '2': res = R2; break;
    case '3': res = R3; break;
    case '4': res = R4; break;
    case '5': res = R5; break;
    case '6': res = R6; break;
    case '7': res = R7; break;
    case '8': res = R8; break;
    case '9': res = R9; break;
    case 'C': res = PC; break;
    case 'P': res = CPSR; break;
    default: res = 0; printf("REGISTER ERROR, REGISTER NON EXISTANT");
  }
  return res;
}


/*
 *  @param: the opcode token including the suffix 
 *  @return: the condition related with the opcodes' suffix
 */
uint32_t getCond(char *opcode){
  int len = strlen(opcode);
  if(len <= 3 || opcode[1] == 'b'){
    return 0xe;
  }
  
  char *suffix = opcode + 3 * sizeof(char);
  uint32_t *out;
  if((out = getElem(code_binarycode,suffix)) == NULL){
    printf("SuffixError, suffix pattern non-existant");
    exit(EXIT_FAILURE);
  }
  return *out;
}


/*Takes two ints and returns the larger*/
int max(int a, int b) {
	if (a > b) {
		return a;
	} else {
		return b;
	}
}

/*This function returns 1 if the function has 24(or more) consecutive 0s 
 *(including wrap around) and returns 0 otherwise.*/
int numberOfZeros(uint32_t extractedExp) {
	uint32_t mask = 0x00FFFFFFFF;
	int i;
	for (i = 0; i < 32; i++) {
		if ((mask & extractedExp) == 0) {
			return 1;
		}
	}
	return 0;
}

uint32_t execute_rotate_left(uint32_t shift_amount, uint32_t reg_val) {
	return ((reg_val << shift_amount) | (reg_val >> (32 - shift_amount)));
}

/*Takes in a 32 bit unisgned vlaue and returns a 32 bit unsigned int
 *which represents the rotate-Immediate value representation of the input as
 *descried on page 7 of the spec. (the first 20 bits of the return value will 
 *be 0
 *@Param extractedExp is the value to be converted*/
uint32_t convertToImm(uint32_t extractedExp) {
	printf("extractedExp  = %x\n", extractedExp);
	printBits(extractedExp);
	uint32_t rotated;
	uint32_t i;
	for (i = 0; i < 16; i++) {
		rotated = execute_rotate_left(2 * i, extractedExp);
		//   printBits(rotated);
		if (rotated < 256) {
			return i << 8 | rotated;
		}
	}
	return 0xFFFFFFFF; // this is returned if number is irepresentable
}

/* char *source is the string of words of the instructions */
/*There is a lot of redundancy her that we could proably clean up*/
// input source includes space at beginning for example is " lsr r6" or " asr #6" or " ror #0xA"
// post : outputs bits 11 - 4 of instruction
uint32_t calculateShift(char *source) {
	const char delim[3] = " ,";
	char *shifttype = strtok(source, delim);
	char *expr = strtok(NULL, delim);
	int imm;

	if (expr == NULL) {
		printf("REG PARAMETER NON-EXISTANT\n");
		exit(EXIT_FAILURE);
	}
	if (expr[0] == '#') {
		imm = 1;
	} else if (expr[0] == 'r') {
		imm = 0;
	} else {
		perror("Shift command doesn't exist");
		exit(EXIT_FAILURE);
	}

//	uint32_t extractednum = extractNum(expr);
//	if (extractednum >= 32) {
//		printf("rotation too large\n");
//		exit(EXIT_FAILURE);
//	}
	uint32_t res;
	uint32_t shifttypeint;
	uint32_t immval;
	uint32_t regval;
	if (shifttype[0] == 'l') {
		if (shifttype[2] == 'l') {
			shifttypeint = 0x0;
			if (imm) {
				immval = extractNum(expr);
				res = immval << 7 | shifttypeint << 5;
			} else {
				regval = toCpuReg(expr);
				res = regval << 8 | shifttypeint << 5 | 1 << 4;
			}
		} else if (shifttype[2] == 'r') {
			shifttypeint = 0x1;
			if (imm) {
				immval = extractNum(expr);
				res = immval << 7 | shifttypeint << 5;
			} else {
				regval = toCpuReg(expr);
				res = regval << 8 | shifttypeint << 5 | 1 << 4;
			}
		} else {
			perror("Shift command doesn't exist");
			exit(EXIT_FAILURE);
		}
	} else if (shifttype[0] == 'a') {
		shifttypeint = 0x2;
		if (imm) {
			immval = extractNum(expr);
			res = immval << 7 | shifttypeint << 5;
		} else {
			regval = toCpuReg(expr);
			res = regval << 8 | shifttypeint << 5 | 1 << 4;
		}
	} else if (shifttype[0] == 'r') {
		shifttypeint = 0x3;
		if (imm) {
			immval = extractNum(expr);
			res = immval << 7 | shifttypeint << 5;
		} else {
			regval = toCpuReg(expr);
			res = regval << 8 | shifttypeint << 5 | 1 << 4;
		}
	} else {
		perror("Shift command doesn't exist");
		exit(EXIT_FAILURE);
	}
	return res;
}

/* Translates and, eor, sub, rsb, add ,orr instructions*/
uint32_t *dataProcessing1(char *source) {
	uint32_t *res = malloc(sizeof(uint32_t *));
	assert(source!= NULL);
//	uint32_t first12bits;
	char *rn;
	char *rd;
	uint32_t rotAndImm;
	// uint32_t finalOperand2 = 0;
	char *operand2;
	char *operand2shift;
	const char delim[3] = " ,";
	char *opcode = strtok(source, delim);
	if (opcode == NULL) {
		printf("OPCODE PARAMETER NON-EXISTANT");
		exit(EXIT_FAILURE);
	}
  uint32_t cond = getCond(opcode);
  if(cond != 0xe){
    opcode[3] = '\0';
  }
	uint32_t *opcodeint = (uint32_t *)getElem(code_binarycode, opcode);
	uint32_t imm = 0x1;

	if ((rn = strtok(NULL, delim)) == NULL) {
		printf("OPCODE PARAMETER NON-EXISTANT");
		exit(EXIT_FAILURE);
	}
	if ((rd = strtok(NULL, delim)) == NULL) {
		printf("OPCODE PARAMETER NON-EXISTANT");
		exit(EXIT_FAILURE);
	}

	/* Get operand2 */
	if ((operand2 = strtok(NULL, delim)) == NULL) {
		printf("OPCODE PARAMETER NON-EXISTANT");
		exit(EXIT_FAILURE);
	}
	uint32_t rnInt = toCpuReg(rn);
	//   	    printf("rnInt = %x\n", *rnInt);
	uint32_t rdInt = toCpuReg(rd);
	//   	    printf("rdInt = %x\n", *rdInt);

	const char delimshift[3] = ",";
	operand2shift = strtok(NULL, delimshift);

	/* Check if operand 2 is an immediateValue */
	if (operand2[0] == '#') {
		uint32_t *temp = malloc(sizeof(uint32_t *));

		*temp = extractNum(operand2);
		rotAndImm = convertToImm(*temp);
		free(temp);
	}
	/* Else It is a shift */
	else {
		if (operand2[0] == 'r') {
			imm = 0x0;
			char *temp = malloc(sizeof(char *));
			sscanf(operand2, "%s\n", temp);

			uint32_t roInt = toCpuReg(temp);
			//   	    printf("roInt = %x\n", *roInt);
			if (operand2shift == NULL) {
				*res = cond << 28 | imm << 25 | *opcodeint << 21 | rdInt << 16 | rnInt << 12 | roInt;
				free(temp);
				return res;
			} else {
				printf("operand2shift \"%s\"\n", operand2shift);
				*res = cond << 28 | imm << 25 | *opcodeint << 21 | calculateShift(operand2shift) | rdInt << 16 | rnInt << 12 | roInt;
				return res;
			}
		}
	}
	*res = cond << 28 | imm << 25 | *opcodeint << 21 | rdInt << 16 | rnInt << 12 | rotAndImm;
	return res;
}

/* Translates mov */
uint32_t *dataProcessing2(char *source) {

	uint32_t *res = malloc(sizeof(uint32_t *));
 

  const char delim[3] = " ,";
  source = strtok(source, delim);
  uint32_t cond = getCond(source);
  if(cond != 0xe){
    source[3] = '\0';
  }
	uint32_t *opcodeint = (uint32_t *)getElem(code_binarycode, source);
	uint32_t imm = 0x1;

	printf("\nNOW IN DP2\nThis is your input = %s\n", source);
	assert(source != NULL);

	char *reg;
	if ((reg = strtok(NULL, delim)) == NULL) {
		printf("OPCODE PARAMETER NON-EXISTANT");
		exit(EXIT_FAILURE);
	}

	char *operand2;
	if ((operand2 = strtok(NULL, delim)) == NULL) {
		printf("OPCODE PARAMETER NON_EXISTANT");
		exit(EXIT_FAILURE);
	}
	uint32_t operandBits;

	/*May want to add the optional shift here*/
	if (operand2[0] == '#') { //has the form #expression
//		uint32_t *temp = malloc(sizeof(uint32_t *));
		uint32_t temp;
		//sscanf(operand2, "#%x" ,temp);
		temp = extractNum(operand2);
		printf("extractedNumber = %x\n", temp);

		/*Now need to convert to from described in emulate*/
		operandBits = convertToImm(temp);
//		free(temp);

	} else {
		/*We can include the case for a shifted register here if we choose to*/
		imm = 0x0;
		char *temp = malloc(sizeof(char *));
		sscanf(operand2, "%s\n", temp);
		operandBits = toCpuReg(temp);
		free(temp);
	}
	uint32_t regint = toCpuReg(reg);
	*res = cond << 28 | imm << 25 | *opcodeint << 21 | regint << 12 | operandBits;
	return res;
}

/* Translates tst, teq, cmp */
uint32_t *dataProcessing3(char *source) {

	uint32_t *res = malloc(sizeof(uint32_t *));
	assert(source != NULL);
	uint32_t *opcodeint;
	uint32_t imm = 0x1;
	char *rn;
	uint32_t rotAndImm;
	char *operand2;

	const char delim[3] = " ,";
	char *opcode = strtok(source, delim);
  uint32_t cond = getCond(opcode);
	if (opcode == NULL) {
		printf("OPCODE PARAMETER NON-EXISTANT");
		exit(EXIT_FAILURE);
	}

  if(cond != 0xe){
    opcode[3] = '\0';
  }
	opcodeint = (uint32_t *)getElem(code_binarycode, opcode);

	if ((rn = strtok(NULL, delim)) == NULL) {
		printf("OPCODE PARAMETER NON-EXISTANT");
		exit(EXIT_FAILURE);
	}
	if ((operand2 = strtok(NULL, delim)) == NULL) {
		printf("OPCODE PARAMETER NON-EXISTANT");
		exit(EXIT_FAILURE);
	}
	if (operand2[0] == '#') {
		uint32_t *temp = malloc(sizeof(uint32_t *));
		sscanf(operand2, "#%x", temp);
		rotAndImm = convertToImm(*temp);
		free(temp);
	} else {
		imm = 0x0;
		char *temp = malloc(sizeof(char *));
		sscanf(operand2, "%s\n", temp);
		uint32_t operandBits = toCpuReg(temp);
		free(temp);
		uint32_t rnInt = toCpuReg(rn);
		//      	printf("rnInt = %x\n", *rnInt);
//       	printf("operandBits = %x\n", *operandBits);
		*res = cond<<28 | 0x1 << 20 | imm << 25 | *opcodeint << 21 | rnInt << 16 | operandBits;
		return res;
	}
	uint32_t rnInt = toCpuReg(rn);
	*res = cond<<28 | 0x31 << 20 | imm << 25 | *opcodeint << 21 | rnInt << 16 | rotAndImm;
	return res;
}

/* Translates mul */
uint32_t *multiply(char *source) {

	uint32_t *res = malloc(sizeof(uint32_t *));
	assert(source!=NULL);

	const char p[3] = " ,";

	//Remove function name 'mul'
	char *token = strtok(source, p);
  uint32_t cond = getCond(token);

	//Parse source string
	char *rd;
	char *rm;
	char *rs;

	if ((rd = strtok(NULL, p)) == NULL) {
		printf("OPCODE PARAMETER NONEXISTANT");
		exit(EXIT_FAILURE);
	}
	if ((rm = strtok(NULL, p)) == NULL) {
		printf("OPCODE PARAMETER NONEXISTANT");
		exit(EXIT_FAILURE);
	}
	if ((rs = strtok(NULL, p)) == NULL) {
		printf("OPCODE PARAMETER NONEXISTANT");
		exit(EXIT_FAILURE);
	}

	//Create uint32_t binary format
	uint32_t rdb = toCpuReg(rd);
	uint32_t rmb = toCpuReg(rm);
	char *temp = malloc(sizeof(char *));
	sscanf(rs, "%s\n", temp);
	uint32_t rsb = toCpuReg(temp);
	uint32_t s = 0x0;
	uint32_t a = 0x0;

  //uint32_t cond = 0xe;
	uint32_t k = 0x9;

	*res = cond << 28 | a << 21 | s << 20 | rdb << 16 | rsb << 8
			| k << 4 | rmb;

	free(temp);
	return res;
}

/* Translates mla */
uint32_t *multiplyAccum(char *source) {
	assert(source!=NULL);
	uint32_t *res = malloc(sizeof(uint32_t *));
	uint32_t *binary = multiply(source);
	char *rn;

	if ((rn = strtok(NULL, " ,")) == NULL) {
		printf("OPCODE PARAMETER NONEXISTANT");
		exit(EXIT_FAILURE);
	}

	uint32_t rnb = toCpuReg(rn);
	uint32_t a = 0x1;

	*binary = binaryReplace(rnb, 4, binary, 12);
	*res = binaryReplace(a, 1, binary, 21);
	free(binary);
	return res;
}

/* Translates ldr */
uint32_t *sdt_ldr(char *source) {
	uint32_t *res = malloc(sizeof(uint32_t *));
	printf("source = %s\n", source);
	const char p[3] = " ,";
	const char delimshift[3] = ",";
  uint32_t cond = getCond(strtok(source, p));
	uint32_t I = 0x0;
	uint32_t P = 0x1;
	uint32_t U = 0x1;

	char *rd;
	if ((rd = strtok( NULL, p)) == NULL) {
		printf("OPCODE PARAMETER NONEXISTANT");
		exit(EXIT_FAILURE);
	}
	char *expr;
	if ((expr = strtok( NULL, p)) == NULL) {
		printf("OPCODE PARAMETER NONEXISTANT");
		exit(EXIT_FAILURE);
	}
	char *expr2;
	expr2 = strtok(NULL, p);
	printf("This is expr2 \"%s\"\n", expr2);

	char *op2shift;
	op2shift = strtok(NULL, delimshift);
	printf("This is expr : expr2 : expr3 = %s : %s : %s\n", expr, expr2,
			op2shift);
	uint32_t rdint = toCpuReg(rd);
	//1. Expr is numeric constant of form '=#'

	if (expr[0] == '=') {

		sscanf(expr, "=%x", res);
		if (*res < 0xff) {
			//effectively a move
			*res = cond << 28 | 0x3A00000 | rdint << 12 | *res;
		} else {
			uint32_t offset = 0x0;
			if (file_length + getNumElems(LDRconsts) - file_line >= 2) {
				offset = 4
						* (file_length - file_line - 1 + getNumElems(LDRconsts)
								- getNumElems(label_address));
			}
			printf("This is getNumElems(LDRconsts) %x\n",
					getNumElems(LDRconsts));
			printf("This is offset %x\n", offset);
			char *str = malloc(sizeof(char *));
			uint32_t *ldrconstant = malloc(sizeof(uint32_t *));
			*ldrconstant = *res;
			sprintf(str, "%x", file_line);
			printf("This is str %s\n", str);
			if (putElem(LDRconsts, str, (void *)ldrconstant) == 0) {
				perror("Couldn't insert LDRconstant into dictionary");
				exit(EXIT_FAILURE);
			}
			I = 0x0; P = 0x1; U = 0x1;

			*res = cond << 28 | 0x01F << 16 | 1 << 26 | I << 25 | P << 24 | U << 23 | rdint << 12 | offset;
		}
	} else {
		I = 0x0; P = 0x1; U = 0x1;
		char *newexpr = malloc(sizeof(char *));
		char *t2 = malloc(sizeof(char *));
		sscanf(expr, "[%[^]] %[]] ", newexpr, t2);
		printf("this is temp1 %s\n", newexpr);
		uint32_t rnint = toCpuReg(newexpr);
		free(t2);
		if (expr2 == NULL) {
			*res = cond << 28 | 0x41 << 20 | I << 25 | P << 24 | U << 23 | rnint << 16 | rdint << 12;
		} else {
			uint32_t *temp = malloc(sizeof(uint32_t *));
			if (expr2[0] == '#') {
				*temp = extractNum(expr2);
				if (strchr(expr2, ']') == NULL) {
					I = 0x0; P = 0x0; U = 0x1;
					if (isNegative(*temp)) {
						*temp = flipSign(*temp);
						I = 0x0; P = 0x1; U = 0x0;
					}
				} else {
					if (isNegative(*temp)) {
						*temp = flipSign(*temp);
						I = 0x0; P = 0x1; U = 0x0;
					}
				}
				printf("this is temp %x \n", *temp);
				*res = cond <<28 | 0x41 << 20 | I << 25 | P << 24 | U << 23 | rnint << 16 | rdint << 12 | *temp;
			} else {
				uint32_t roff;
				I = 0x1; P = 0x1; U = 0x1;
				if (op2shift != NULL) {
					op2shift[strlen(op2shift) - 1] = '\0';
					printf("This is expr2 \"%s\"\n", expr2);
					roff = toCpuReg(expr2);
					printf("This is op2shift %s\n", op2shift);
					*res = 0xE << 28 |0x41 << 20 | I << 25 | P << 24 | U << 23 | rnint << 16 | rdint << 12 | roff
							| calculateShift(op2shift);
				} else {
					if (strchr(expr2, ']') != NULL) {
						expr2[strlen(expr2) - 1] = '\0';
						roff = toCpuReg(expr2);
					} else {
						I = 0x1; P = 0x0; U = 0x1;
						roff = toCpuReg(expr2);
					}
					printf("This is roff %x\n", roff);
					*res = 0xE << 28 | 0x41 << 20 | I << 25 | P << 24 | U << 23 | rnint << 16 | rdint << 12 | roff;
				}
			}
		}
	}
	return res;
}

uint32_t twentieth0(uint32_t num) {
	printf("num in twentieth0 first = %x\n", num);
	if (num && 0x100000 == 0x100000) {
		num -= 0x100000;
	}
	printf("num in twentieth0 = %x\n", num);
	return num;
}

/* Translates str */
uint32_t *sdt_str(char *source) {
	//Same as ldr but with 20th bit replaced with a 0;
	uint32_t *binary = sdt_ldr(source);
	*binary = twentieth0(*binary);
	return binary;
}

uint32_t *branchAux(uint32_t *code, uint32_t *offset) {
	uint32_t *res = malloc(sizeof(uint32_t *));
	*res = *code << 28 | 0xA << 24 | *offset;
	return res;
}

/* Translates branch - beq, bne, bge, blt, bgt, ble, b, */
uint32_t *branch(char *source) {
	assert(source != NULL);
	const char p[3] = " ,:";
	char *token = strtok(source, p);
	printf("branch token = %s\n", token);

	uint32_t *binary = malloc(sizeof(uint32_t *));

	//Get Code
	printf("this is the value of b: %p\n", getElem(code_binarycode, token));
	uint32_t *code = (uint32_t *) getElem(code_binarycode, token);
	printf("this is cond_b = %x\n", *code);

	//Get Expression
	uint32_t *labelAddress;

	if ((token = strtok(NULL, p)) == NULL) {
		printf("OPCODE PARAMETER NONEXISTANT");
		exit(EXIT_FAILURE);
	}
	char *temp = malloc(sizeof(char *));
	sscanf(token, " %s:", temp);

	//Get Label Value
	if ((labelAddress = (uint32_t *) getElem(label_address, temp)) == NULL) {
		printf("LABEL NONEXISTANT");
		exit(EXIT_FAILURE);
	}
	free(temp);
	uint32_t *offset = malloc(sizeof(uint32_t));
	printf("labelAddress  %x, file_line %x\n", *labelAddress, file_line);
	//lines of code offset
	*offset = *labelAddress - file_line;
	if (*labelAddress > file_line && *labelAddress - file_line <= 2) {
		*offset -= 2;    // if condtion e.g. b label is above :label(jump ahead)
	} else if (file_line > *labelAddress && file_line - *labelAddress <= 5) {
		*offset -= 1;
	}
	printf("offset = %x\n", *offset);

	//either add 1 or 2 lines
	*offset = *offset & 0x00FFFFFF;
	//*offset = *offset  2;
	printf("offset2 = %x\n", *offset);
	*binary = (*offset >> 2);

	uint32_t *res = branchAux(code, offset);
	printf("res = %x\n", *res);
	free(offset);
	free(binary);

	return res;
}

/* Translates special - andeq */
uint32_t *spec_andeq(char *source) {
	//Returns value 0x0
	uint32_t *res = malloc(sizeof(uint32_t *));
	*res = 0x0;
	return res;
}

/* Translates special - lsl */
uint32_t *spec_lsl(char *source) {

	uint32_t *res = malloc(sizeof(uint32_t *));

	uint32_t base = 0xE1A00000;

	const char p[3] = " ,";
	strtok(source, p);

	char *reg = strtok(NULL, p);
	char *expr = strtok(NULL, p);

	if (reg == NULL) {
		printf("REG PARAMETER NON-EXISTANT\n");
		exit(EXIT_FAILURE);
	}
	uint32_t regint = toCpuReg(reg);

	uint32_t extractednum = extractNum(expr);
	if (extractednum >= 32) {
		printf("rotation too large\n");
		exit(EXIT_FAILURE);
	}
	*res = (base | regint << 12 | extractednum << 7 | regint);
	return res;
}
