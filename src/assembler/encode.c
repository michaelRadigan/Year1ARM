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

// extracted uint32_t from string in form "#2342" or "#0x23EF8"
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

/* 
 Sets up structs containing function pointers
 This is a stupid method but cannot think of
 anything better at the moment
 */
void setUPFuncStructs(void) {
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

void setUPcode_binarycode(void) {
	DICTIONARY *d = createDictionary();
//  BR *codes = malloc(sizeof(BR));
	//Opcodes for binaryConditions

	uint32_t *beq = malloc(sizeof(uint32_t *));
	*beq = 0x0;
	uint32_t *bne = malloc(sizeof(uint32_t *));
	*bne = 0x1;
	uint32_t *bge = malloc(sizeof(uint32_t *));
	*bge = 0xa;
	uint32_t *blt = malloc(sizeof(uint32_t *));
	*blt = 0xb;
	uint32_t *bgt = malloc(sizeof(uint32_t *));
	*bgt = 0xc;
	uint32_t *ble = malloc(sizeof(uint32_t *));
	*ble = 0xd;
	uint32_t *b = malloc(sizeof(uint32_t *));
	*b = 0xe;
	putElem(d, "beq", beq);  //eq
	putElem(d, "bne", bne);  //ne
	putElem(d, "bge", bge);  //ge
	putElem(d, "blt", blt);  //lt
	putElem(d, "bgt", bgt);  //gt
	putElem(d, "ble", ble);  //le

	putElem(d, "b", b);  //al

	uint32_t *and = malloc(sizeof(uint32_t *));
	*and = 0x0;
	uint32_t *eor = malloc(sizeof(uint32_t *));
	*eor = 0x1;
	uint32_t *sub = malloc(sizeof(uint32_t *));
	*sub = 0x2;
	uint32_t *rsb = malloc(sizeof(uint32_t *));
	*rsb = 0x3;
	uint32_t *add = malloc(sizeof(uint32_t *));
	*add = 0x4;
	uint32_t *orr = malloc(sizeof(uint32_t *));
	*orr = 0xc;
	uint32_t *mov = malloc(sizeof(uint32_t *));
	*mov = 0xd;
	uint32_t *tst = malloc(sizeof(uint32_t *));
	*tst = 0x8;
	uint32_t *teq = malloc(sizeof(uint32_t *));
	*teq = 0x9;
	uint32_t *cmp = malloc(sizeof(uint32_t *));
	*cmp = 0xa;

	//Opcodes for Data Processing
	putElem(d, "and", and);
	putElem(d, "eor", eor);
	putElem(d, "sub", sub);
	putElem(d, "rsb", rsb);
	putElem(d, "add", add);
	putElem(d, "orr", orr);
	putElem(d, "mov", mov);
	putElem(d, "tst", tst);
	putElem(d, "teq", teq);
	putElem(d, "cmp", cmp);

	code_binarycode = d;
}

void setUpLDRconsts(void) {
	LDRconsts = createDictionary();
}

void destroyLDRconsts(void) {
	destroyDictionary(LDRconsts);
}

//void setUPregistervalue_dict(void) {
//	regist_vals = createDictionary();
//}

//void destroyregistervalue_dict(void) {
//	destroyDictionary(regist_vals);
//}

void setUPregister_dict(void) {
	DICTIONARY *d = createDictionary();

	//Opcodes for registers
	uint32_t *r0 = malloc(sizeof(uint32_t *));
	*r0 = 0x0;
	uint32_t *r1 = malloc(sizeof(uint32_t *));
	*r1 = 0x1;
	uint32_t *r2 = malloc(sizeof(uint32_t *));
	*r2 = 0x2;
	uint32_t *r3 = malloc(sizeof(uint32_t *));
	*r3 = 0x3;
	uint32_t *r4 = malloc(sizeof(uint32_t *));
	*r4 = 0x4;
	uint32_t *r5 = malloc(sizeof(uint32_t *));
	*r5 = 0x5;
	uint32_t *r6 = malloc(sizeof(uint32_t *));
	*r6 = 0x6;
	uint32_t *r7 = malloc(sizeof(uint32_t *));
	*r7 = 0x7;
	uint32_t *r8 = malloc(sizeof(uint32_t *));
	*r8 = 0x8;
	uint32_t *r9 = malloc(sizeof(uint32_t *));
	*r9 = 0x9;
	uint32_t *r10 = malloc(sizeof(uint32_t *));
	*r10 = 0xa;
	uint32_t *r11 = malloc(sizeof(uint32_t *));
	*r11 = 0xb;
	uint32_t *r12 = malloc(sizeof(uint32_t *));
	*r12 = 0xc;
	uint32_t *pc = malloc(sizeof(uint32_t *));
	*pc = 0xf;
	uint32_t *cpsr = malloc(sizeof(uint32_t *));
	*cpsr = 0x10;

	putElem(d, "r0", (void *) r0);
	putElem(d, "r1", (void *) r1);
	putElem(d, "r2", (void *) r2);
	putElem(d, "r3", (void *) r3);
	putElem(d, "r4", (void *) r4);
	putElem(d, "r5", (void *) r5);
	putElem(d, "r6", (void *) r6);
	putElem(d, "r7", (void *) r7);
	putElem(d, "r8", (void *) r8);
	putElem(d, "r9", (void *) r9);
	putElem(d, "r10", (void *) r10);
	putElem(d, "r11", (void *) r11);
	putElem(d, "r12", (void *) r12);
	putElem(d, "pc", (void *) pc);
	putElem(d, "cpsr", (void *) cpsr);

	register_dict = d;
}

void destroyRegisterDictionary(void) {
	destroyDictionary(register_dict);
}

void destroycode_binarycode(void) {
	destroyDictionary(code_binarycode);
	// free(BR);
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
uint32_t *binaryConcat(uint32_t *b1, uint32_t *b2, int pos) {
	//PRE: After pos in b2, there exist only 0 bits.
//  uint32_t (*ptr)(uint32_t,uint32_t, int) = &binaryConcatHelper;

//	*ptr = binaryConcatHelper(b1, b2, pos);
//	  printf("Printing tree in binaryConcat....\n");
//	  printPaths(label_address->tree);
	uint32_t num = (*b2 | (*b1 << pos));
	uint32_t *ptrhelp = &num;
	return ptrhelp;
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

uint32_t *binaryReplace(uint32_t *b1, int numberOfBits, uint32_t *b2, int pos) {
	//replaces the bit of b2 to be replaced with 0s
	uint32_t mask = 1;
	for (int i = 0; i < numberOfBits - 1; i++) {
		mask = 1 + (mask << 1); // put 1s in correct place
	}
	*b2 = *b2 & ~(mask << pos); // and so 0s are in swap position
	return binaryConcat(b1, b2, pos);
}

/*
 * Converts a lowerCase register name to the format
 * specified in cpu_reg of cpu.h (reuse of code)
 * @param str: the name of the register e.g. r2 or r8
 * @return: pointer to the cpu_reg version of the register name.
 */

uint32_t *toCpuReg(char *str) {

	/*  //only the beginning r will ever be lowercase
	 str[0] = tolower(str[0]);
	 uint32_t *ret;

	 if((ret = getElem(register_dict,str))==NULL){
	 printf("ILLEGAL REGISTER\n");
	 exit(EXIT_FAILURE);
	 }

	 return ret;*/

	str[0] = toupper(str[0]);
	uint32_t *num = malloc(sizeof(uint32_t *));
	uint32_t *res = num;
	//  uint32_t  *res;
	// return (cpu_reg *)str;
	if (strcmp(str, "R0") == 0) {
		*res = R0;
	} else if (strcmp(str, "R1") == 0) {
		*res = R1;
	} else if (strcmp(str, "R2") == 0) {
		*res = R2;
	} else if (strcmp(str, "R3") == 0) {
		*res = R3;
	} else if (strcmp(str, "R4") == 0) {
		*res = R4;
	} else if (strcmp(str, "R5") == 0) {
		*res = R5;
	} else if (strcmp(str, "R6") == 0) {
		*res = R6;
	} else if (strcmp(str, "R7") == 0) {
		*res = R7;
	} else if (strcmp(str, "R8") == 0) {
		*res = R8;
	} else if (strcmp(str, "R9") == 0) {
		*res = R9;
	} else if (strcmp(str, "R10") == 0) {
		*res = R10;
	} else if (strcmp(str, "R11") == 0) {
		*res = R11;
	} else if (strcmp(str, "R12") == 0) {
		*res = R12;
	} else if (strcmp(str, "PC") == 0) {
		*res = PC;
	} else if (strcmp(str, "CPSR") == 0) {
		*res = CPSR;
	}
	//free(num);
	return res;
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
	uint32_t *regval;
	if (shifttype[0] == 'l') {
		if (shifttype[2] == 'l') {
			shifttypeint = 0x0;
			if (imm) {
				immval = extractNum(expr);
				res = immval << 7 | shifttypeint << 5;
			} else {
				regval = toCpuReg(expr);
				res = *regval << 8 | shifttypeint << 5 | 1 << 4;
			}
		} else if (shifttype[2] == 'r') {
			shifttypeint = 0x1;
			if (imm) {
				immval = extractNum(expr);
				res = immval << 7 | shifttypeint << 5;
			} else {
				regval = toCpuReg(expr);
				res = *regval << 8 | shifttypeint << 5 | 1 << 4;
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
			res = *regval << 8 | shifttypeint << 5 | 1 << 4;
		}
	} else if (shifttype[0] == 'r') {
		shifttypeint = 0x3;
		if (imm) {
			immval = extractNum(expr);
			res = immval << 7 | shifttypeint << 5;
		} else {
			regval = toCpuReg(expr);
			res = *regval << 8 | shifttypeint << 5 | 1 << 4;
		}
	} else {
		perror("Shift command doesn't exist");
		exit(EXIT_FAILURE);
	}
	return res;
}

/* Translates and, eor, sub, rsb, add ,orr instructions*/
uint32_t *dataProcessing1(char *source) {
	assert(source!= NULL);
	uint32_t first12bits;
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
	/*
	 Convert opcode to binary
	 if((first12bits =(uint32_t *) getElem(code_binarycode,opcode)) == NULL){
	 printf("OPCODE IS NOT OF THE RIGHT FORMAT\n");
	 exit(EXIT_FAILURE);
	 }*/

	/*The 2nd letter of each mnemonic is unique, so we will just test that*/
	if (opcode[1] == 'n') {
		first12bits = 0xE2000000; // This will have to be changed if we decide
								  // decide to implement th eoptional shifts
	} else if (opcode[1] == 'o') {
		first12bits = 0xE2200000; // ^^^^^^^^
	} else if (opcode[1] == 'u') {
		first12bits = 0xE2400000; // ^^^^^^^^
	} else if (opcode[1] == 's') {
		first12bits = 0xE2600000; // ^^^^^^^^
	} else if (opcode[1] == 'd') {
		first12bits = 0xE2800000; // ^^^^^^^^
	} else if (opcode[1] == 'r') {
		first12bits = 0xE3800000; // ^^^^^^^^
	}
	/* Create first 12 bits */
	//*first12bits = (*first12bits << 21) | 0xE0000000;
	/* Get registers */
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
	const char delimshift[3] = ",";
	operand2shift = strtok(NULL, delimshift);

	/* Check if operand 2 is an immediateValue */
	if (operand2[0] == '#') {
		uint32_t *temp = malloc(sizeof(uint32_t *));
//        sscanf(operand2, "#%x", temp);
		*temp = extractNum(operand2);
		/*<<<<<<< HEAD
		 printf("temp val = %x\n", *temp);
		 uint32_t k = 0x1;
		 first12bits = binaryReplace(&k,1,first12bits,25);
		 finalOperand2= convertToImm(*temp);
		 =======*/
		//       printf("temp val = %x\n", *temp);
		rotAndImm = convertToImm(*temp);
		free(temp);
	}
	/* Else It is a shift */
	else {
		//THIS DOESNT ACTUALLY DO THE SHIFT!!
		if (operand2[0] == 'r') {
			if (opcode[1] == 'n') {
				first12bits = 0xE0000000;

			} else if (opcode[1] == 'o') {
				first12bits = 0xE0200000;
			} else if (opcode[1] == 'u') {
				first12bits = 0xE0400000;
			} else if (opcode[1] == 's') {
				first12bits = 0xE0600000;
			} else if (opcode[1] == 'd') {
				first12bits = 0xE0800000;
			} else if (opcode[1] == 'r') {
				first12bits = 0xE1800000;
			}

			char *temp = malloc(sizeof(char *));
			sscanf(operand2, "%s\n", temp);
			uint32_t *rnInt = (uint32_t *) toCpuReg(rn);
			//   	    printf("rnInt = %x\n", *rnInt);
			uint32_t *rdInt = (uint32_t *) toCpuReg(rd);
			//   	    printf("rdInt = %x\n", *rdInt);
			uint32_t *roInt = (uint32_t *) toCpuReg(temp);
			//   	    printf("roInt = %x\n", *roInt);
			if (operand2shift == NULL) {
				*rdInt =
						(first12bits | (*rdInt) << 16 | (*rnInt) << 12 | *roInt);
				free(temp);
				return rdInt;
			} else {
				printf("operand2shift \"%s\"\n", operand2shift);
				*rdInt = first12bits | calculateShift(operand2shift)
						| *rdInt << 16 | *rnInt << 12 | *roInt;
				return rdInt;

			}
		}
	}
//>>>>>>> origin/Oliver

	/* Finishing off */
	uint32_t *rnInt = (uint32_t *) toCpuReg(rn);
//    printf("rnInt = %x\n", *rnInt);
	uint32_t *rdInt = (uint32_t *) toCpuReg(rd);
	/*<<<<<<< HEAD
	 printf("rdInt = %x\n", *rdInt);
	 printf("finalOperand2 = %x\n", finalOperand2);
	 *rdInt = (*first12bits | (*rdInt) << 16 | (*rnInt) << 12 | finalOperand2);
	 =======*/
//    printf("rdInt = %x\n", *rdInt);
	//   printf("rotAndImm = %x\n", rotAndImm);
	*rdInt = (first12bits | (*rdInt) << 16 | (*rnInt) << 12 | rotAndImm);
//>>>>>>> origin/Oliver
	return rdInt;
}

/* Translates mov */
uint32_t *dataProcessing2(char *source) {
	/*<<<<<<< HEAD

	 assert(source != NULL);
	 const char delim[3] = " ,";
	 source = strtok(source, delim);

	 =======*/

	printf("\nNOW IN DP2\nThis is your input = %s\n", source);
	assert(source != NULL);
	const char delim[3] = " ,";
	source = strtok(source, delim);
	printf("This is your input after strtok = %s\n", source);
//>>>>>>> origin/Oliver
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
	uint32_t startingBits = 0;
	uint32_t *operandBits = malloc(sizeof(uint32_t *));

	/*May want to add the optional shift here*/
	if (operand2[0] == '#') { //has the form #expression
		startingBits = 0xE3A00000;
//>>>>>>> origin/Oliver
		uint32_t *temp = malloc(sizeof(uint32_t *));
		//sscanf(operand2, "#%x" ,temp);
		*temp = extractNum(operand2);
		printf("extractedNumber = %x\n", *temp);

		/*Now need to convert to from described in emulate*/
		*operandBits = convertToImm(*temp);
		free(temp);

	} else {
		/*We can include the case for a shifted register here if we choose tio*/
		/*<<<<<<< HEAD

		 char *temp = malloc(sizeof(char *));
		 sscanf(operand2, "%s\n", temp);
		 operandBits = (uint32_t *) toCpuReg(temp);
		 free(temp);

		 startingBits = 0xE1A00000;

		 =======*/

		char *temp = malloc(sizeof(char *));
		sscanf(operand2, "%s\n", temp);
		operandBits = (uint32_t *) toCpuReg(temp);
		free(temp);

		startingBits = 0xE1A00000;

//>>>>>>> origin/Oliver
	}
	uint32_t *regint = (uint32_t *) toCpuReg(reg);
	*regint = (startingBits | *regint << 12 | *operandBits);
	free(operandBits);
	return regint;
}

/* Translates tst, teq, cmp */
uint32_t *dataProcessing3(char *source) {
	assert(source != NULL);
	uint32_t first12bits;
	char *rn;
	uint32_t rotAndImm;
	char *operand2;

	const char delim[3] = " ,";
	char *opcode = strtok(source, delim);
	if (opcode == NULL) {
		printf("OPCODE PARAMETER NON-EXISTANT");
		exit(EXIT_FAILURE);
	}
	if (opcode[0] == 'c') {
		first12bits = 0xE3500000; // This will have to be changed if we decide
								  // decide to implement th eoptional shifts
	} else if (opcode[1] == 's') {
		first12bits = 0xE3100000; // ^^^^^^^^
	} else if (opcode[1] == 'e') {
		first12bits = 0xE3300000; // ^^^^^^^^
	} else {
		/*ERROR - passed this function something wrong*/
	}
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
		/*this is where we could put the optional shift stuff*/
		if (opcode[0] == 'c') {
			first12bits = 0xE1500000; // This will have to be changed if we decide
									  // decide to implement th eoptional shifts
		} else if (opcode[1] == 's') {
			first12bits = 0xE1100000; // ^^^^^^^^
		} else if (opcode[1] == 'e') {
			first12bits = 0xE1300000; // ^^^^^^^^
		} else {
			/*ERROR - passed this function something wrong*/
		}
		char *temp = malloc(sizeof(char *));
		sscanf(operand2, "%s\n", temp);
		uint32_t *operandBits = (uint32_t *) toCpuReg(temp);
		free(temp);
		uint32_t *rnInt = (uint32_t *) toCpuReg(rn);
		//      	printf("rnInt = %x\n", *rnInt);
//       	printf("operandBits = %x\n", *operandBits);
		*rnInt = (first12bits | (*rnInt) << 16 | *operandBits);
		return rnInt;
	}
	uint32_t *rnInt = (uint32_t *) toCpuReg(rn);
	*rnInt = (first12bits | (*rnInt) << 16 | rotAndImm);
	return rnInt;
}

/* Translates mul */
uint32_t *multiply(char *source) {
	assert(source!=NULL);
	uint32_t *binary = calloc(1, sizeof(uint32_t *));

	const char p[3] = " ,";
	//Remove function name 'mul'
	strtok(source, p);

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

	*binary = (*cond << 28) | (a << 21) | (s << 20) | (*rdb << 16) | (*rsb << 8)
			| (k_ << 4) | *rmb;

	free(temp);
	free(cond);
	return binary;
}

/* Translates mla */
uint32_t *multiplyAccum(char *source) {
	assert(source!=NULL);
	uint32_t *binary = multiply(source);
	char *rn;

	if ((rn = strtok(NULL, " ,")) == NULL) {
		printf("OPCODE PARAMETER NONEXISTANT");
		exit(EXIT_FAILURE);
	}

	uint32_t *rnb = (uint32_t *) toCpuReg(rn);
	uint32_t *a = malloc(sizeof(uint32_t *));
	*a = 0x1;

	binary = binaryReplace(rnb, 4, binary, 12);
	binary = binaryReplace(a, 1, binary, 21);

	return binary;
}

/* Translates ldr */
uint32_t *sdt_ldr(char *source) {
	printf("source = %s\n", source);
	const char p[3] = " ,";
	const char delimshift[3] = ",";
	//Remove function name 'ldr'
	strtok(source, p);

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

	//3 ways of interpreting expr
	uint32_t *rdint = toCpuReg(rd);
	//1. Expr is numeric constant of form '=#'
	uint32_t startingBits = 0xE3A00000;
	uint32_t *val = malloc(sizeof(uint32_t *));
	if (expr[0] == '=') {
		sscanf(expr, "=%x", val);
		if (*val < 0xff) {
			//effectively a move
			*val = startingBits | *rdint << 12 | *val;
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
			*ldrconstant = *val;
			sprintf(str, "%x", file_line);
			printf("This is str %s\n", str);
			if (putElem(LDRconsts, str, (void *) ldrconstant) == 0) {
				perror("Couldn't insert LDRconstant into dictionary");
				exit(EXIT_FAILURE);
			}

			startingBits = 0xE59F0000;
			*val = startingBits | *rdint << 12 | offset;
		}
	} else {
		startingBits = 0xE5900000;
		char *newexpr = malloc(sizeof(char *));
		char *t2 = malloc(sizeof(char *));
		sscanf(expr, "[%[^]] %[]] ", newexpr, t2);
		printf("this is temp1 %s\n", newexpr);
//			printf("this is temp2 %s\n", t2);
		uint32_t *rnint = toCpuReg(newexpr);
		free(t2);
		if (expr2 == NULL) {
			*val = startingBits | *rnint << 16 | *rdint << 12;
		} else {
//			expr2[strlen(expr2)-1] = '\0';
			uint32_t *temp = malloc(sizeof(uint32_t *));
			if (expr2[0] == '#') {
				*temp = extractNum(expr2);
				if (strchr(expr2, ']') == NULL) {
					startingBits = 0xE4900000;
					if (isNegative(*temp)) {
						*temp = flipSign(*temp);
						startingBits = 0xE5100000;
					}
				} else {
					if (isNegative(*temp)) {
						*temp = flipSign(*temp);
						startingBits = 0xE5100000;
					}
				}
				printf("this is temp %x \n", *temp);
				*val = startingBits | *rnint << 16 | *rdint << 12 | *temp;
			} else {
				uint32_t *roff;
				startingBits = 0xE7900000;
				if (op2shift != NULL) {
					op2shift[strlen(op2shift) - 1] = '\0';
					printf("This is expr2 \"%s\"\n", expr2);
					roff = toCpuReg(expr2);
					printf("This is op2shift %s\n", op2shift);
					*val = startingBits | *rnint << 16 | *rdint << 12 | *roff
							| calculateShift(op2shift);

				} else {
					if (strchr(expr2, ']') != NULL) {
						expr2[strlen(expr2) - 1] = '\0';
						roff = toCpuReg(expr2);
					} else {
						startingBits = 0xE6900000;
						roff = toCpuReg(expr2);
					}
					printf("This is roff %x\n", *roff);
					*val = startingBits | *rnint << 16 | *rdint << 12 | *roff;
				}

			}
		}
	}
	return val;
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
//	else if (file_line > *labelAddress && file_line - *labelAddress >= 4) {
//		*offset -=1;
//	}
//	else if (file_line > *labelAddress && file_line) {
//		*offset -=1;
//	}

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
	uint32_t base = 0xE1A00000;

	const char p[3] = " ,";
	strtok(source, p);

	char *reg = strtok(NULL, p);
	char *expr = strtok(NULL, p);

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
