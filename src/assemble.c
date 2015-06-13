#include "assembler/assemble.h"

/* Structures

 If getting from dictionary, remember to cast back to the right type afterwards

 DICTIONARY *label_address := (char *str , uint32_t *location);
 DICTIONARY *opcode_function := (char *code , (*ptr_func) (char *source));

 */

DICTIONARY *setUPlabel_address(void) {
	DICTIONARY *d = createDictionary();
	return d;
}

DICTIONARY *setUpalias_register(void) {
  DICTIONARY *d = createDictionary();
  return d;
}

DICTIONARY *setUPopcode_function(void) {

	//SetUp structs containing function pointers
	setUPFuncStructs();

	DICTIONARY *d = createDictionary();

	//Functions for data Processing
	putElem(d, "and", (void *) dp1);
	putElem(d, "eor", (void *) dp1);
	putElem(d, "sub", (void *) dp1);
	putElem(d, "rsb", (void *) dp1);
	putElem(d, "add", (void *) dp1);
	putElem(d, "orr", (void *) dp1);
	putElem(d, "mov", (void *) dp2);
	putElem(d, "tst", (void *) dp3);
	putElem(d, "teq", (void *) dp3);
	putElem(d, "cmp", (void *) dp3);

	//Functions for Multiply
	putElem(d, "mul", (void *) m);
	putElem(d, "mla", (void *) ma);

	//Functions for single data transfer
	putElem(d, "ldr", (void *) ldr);
	putElem(d, "str", (void *) str);

	//Functions for branching
	putElem(d, "beq", (void *) b);
	putElem(d, "bne", (void *) b);
	putElem(d, "bge", (void *) b);
	putElem(d, "blt", (void *) b);
	putElem(d, "bgt", (void *) b);
	putElem(d, "ble", (void *) b);
	putElem(d, "b", (void *) b);

	//Functions for specials
	putElem(d, "andeq", (void *) andeq);
	putElem(d, "lsl", (void *) lsl);

	return d;
}

/* Sets up all Dictionry structures */
void setUpDictionaries(){
	label_address = setUPlabel_address();
	setUPcode_binarycode();
  alias_register = setUpalias_register();
	opcode_function = setUPopcode_function();
	setUPregister_dict();
	setUpLDRconsts();
}

/* Frees all memory allocated by the function dictionary */
void destroyDictionaryfunctions(DICTIONARY *d) {
	destroyFuncStructs();
	destroyDictionary(d);
}

/* Frees all dictionaries */
void destroyAllDictionaries(){
	destroyDictionaryKEYS(label_address);
	destroyDictionaryVALUES(label_address); // this is where we free the values we malloced in storeLabel function
	destroyDictionary(label_address);


	destroycode_binarycode();
	destroyDictionaryfunctions(opcode_function);
	destroyRegisterDictionary();
	destroyLDRconsts();
	destroyDictionary(alias_register);

}

/* Checks if label exists, This will store it*/
int storeLabel(char *source) {
	char *t1 = malloc(sizeof(char *));
	char *t2 = malloc(sizeof(char *));
	sscanf(source, "%[^:] %[:] ", t1, t2);

	//Not Label
	if (t2[0] == '\0') {
		free(t1);
		free(t2);
		return 0;
	}
	uint32_t *linenumlabel = malloc(sizeof(uint32_t *));
	*linenumlabel = file_line;

	putElem(label_address, t1, linenumlabel);
	free(t2);
	return 1;
}

/* Removes the label from the source string if it exists */
char *removeLabel(char *source) {
	char *t1 = calloc(1, sizeof(char *));
	char *t2 = calloc(1, sizeof(char *));
	sscanf(source, " %[^:] %*[ :] %[^:]\n\n", t1, t2);
	if (t2[0] == 0 || t2[0] =='\0') {
		free(t2);
		return (t1);
	}
	free(t1);
	return (t2);
}


/* Replaces all aliases in the command with their proper representations */
char *replaceAliases(char *source){
  char *out = malloc(5 * sizeof(source));
  char *c = strtok(source, " ");
  out = strcpy(out,c);
  
  int len = strlen(out);
  while( (c = strtok(NULL," "))!= NULL){
    char *newtoken; // = malloc(sizeof(char));
    sscanf(c , "%[^,#=:<>]" , newtoken);
    char *cp_newToken = malloc(sizeof(char));
    cp_newToken = strcpy(cp_newToken,newtoken);
    
    //Insert space
    out[len++] = ' ';

    //Get possible alias from dictionary  
    if((newtoken = getElem(alias_register,newtoken)) == NULL){
      //if not alias, then concat c back on
    //free(newtoken);
      out = strcat(out, c);
      continue;
    }
    //Insert space

    //Insert chars before alias
    int i = 0;
    for(; c[i] != cp_newToken[0]  ; i++){
      out[len++] = c[i];
    }
    //Insert alias
    for(int j = 0; newtoken[j] != '\0' ; j++){
      out[len++] = newtoken[j];
    }
    //Update search param i by alias length
    i += strlen(cp_newToken);

    //Insert chars after alias
    for(;c[i] != '\0';i++){
      out[len++] = c[i];
    }
    free(cp_newToken);
  }
  free(source);
  return out;
}


/* Writes a unsigned 32 bit number to output stream */
int writeUint32(FILE * const stream, uint32_t value) {
	/* These must be unsigned */
	unsigned char buffer[sizeof(uint32_t)];
	/* Usually 0xFF */
	const unsigned charMask = (1 << CHAR_BIT) - 1;
	for (int i = 0; i < sizeof(buffer); ++i) {
		/* Place the MSB first */
		buffer[sizeof(buffer) - i - 1] = value & charMask;
		value >>= CHAR_BIT;
	}
	int count = fwrite(buffer, sizeof(buffer), 1, stream);
	return count == 1;
}

/* Checks for file existance */
int doesFileExist(const char *filename) {
	struct stat st;
	int result = stat(filename, &st);
	return result == 0;

}

uint32_t LEtoBE(uint32_t word) {
	word = ((word << 8) & 0xFF00FF00) | ((word >> 8) & 0x00FF00FF);
	word = (word << 16) | (word >> 16);
	return word;
}

/* MAIN Program loop */
int main(int argc, char **argv) {
	assert(argc == 3);

	const int MAX_LINE_LENGTH = 511;

	//Setup Dictionaries
  setUpDictionaries();

	//Setup File fields
	FILE *ptr_SourceFile = NULL;
	FILE *ptr_WriteFile = NULL;

	char buff[MAX_LINE_LENGTH];

	ptr_SourceFile = fopen(argv[1], "r");

	if (ptr_SourceFile == NULL || !doesFileExist(argv[1])) {
		printf("SOURCEFILE NON EXISTANT\n");
		return 0;
	}

	//Will create file if non existant
	ptr_WriteFile = fopen(argv[2], "w+");

	printf("GOT HERE\n");

	/* Program Loop 1*/
	/* Creates Dictionary for Labels and Memory Locations */

	file_line = 0;

	while (fgets(buff, MAX_LINE_LENGTH, ptr_SourceFile)) {
		//Check if empty line
		if (buff[0] == '\n' || buff[0] == '\0' || buff[0] == EOF) {
			continue;
		}
		storeLabel(buff);
		file_line++;
	}

	file_length = file_line-1;
	printf("file_length = %x\n", file_length);

	rewind(ptr_SourceFile);
	file_line = 0;

	printf("Beginning Program loop2\n");
	/* Program Loop 2*/
	/* Reads Opcode and generate Binary Encoding */
	while (fgets(buff, MAX_LINE_LENGTH, ptr_SourceFile)) {

	printf("file_line = %x\n", file_line);
    //buff = original line
    //buffer = original line without label, gets strtoked
    //buffTemp = duplicate of buffer
    //token = first elem of buffer, should be opcode.

		//Check if empty line
		if (buff[0] == '\n' || buff[0] == '\0' || buff[0] == EOF) {
	//		file_line++;
			continue;
		}

		//Check if label exists and if so remove it also remove \n
		char *buffer = strtok(buff, "\n");
		buffer = removeLabel(buffer);
    //Buffer now contains no label absolutely

		//Duplicate Buffer
		char *buffTemp;
		if ((buffTemp = malloc(sizeof(buffer))) == NULL) {
			printf("Problem! couldn't allocate memory for buffTemp");
			break;
		}
		buffTemp = strcpy(buffTemp, buffer);

		const char s[2] = " ";
		char *token = strtok(buffer, s);

		printf("\nThis is your buff = '%s'\n", buff);
    printf("This is your token, should be opcode = '%s'\n\n", token);

		//Check if token is a label:
		if (getElem(label_address, (void *) token) != NULL) {
			file_line++;
			continue;
		}
    
    //Check if alias command
    if(strchr(buffTemp , '.') != NULL){
      //Set Alias
      char *reg;
      if(strcmp( reg = strtok(NULL,s) , ".req") == 0){
        reg = strtok(NULL,s);
        putElem(alias_register,token,reg);
      }
      //Remove Alias
      else if(strcmp( token , ".unreq" ) == 0){
        if(!removeElem(alias_register , reg)){
          printf("ALIAS Does not exist!");
          break;
        }
      }
      file_line++;
      continue;
    }

		//Loop-up Opcode to get function
		STR_ENC *encodingStruct;

		if ((encodingStruct = (STR_ENC *) getElem(opcode_function,
				(void *) token)) == NULL) {
			printf("FAILURE FROM DICTIONARY, OPCODE DOES NOT EXIST: %s\n",
					token);
			exit(EXIT_FAILURE);
		}

    //Replace all aliases
//    buffTemp = replaceAliases(buffTemp);

		uint32_t *output = encodingStruct->encFunc(buffTemp);

		*output = LEtoBE(*output);
  	writeUint32(ptr_WriteFile, *output);
		free(buffTemp);
		free(buffer);
		free(output);
		file_line++;
	}

	uint32_t *constant;
	while (!isEmpty(LDRconsts)) {
		constant = (uint32_t *)removeLowestElem(LDRconsts);
		*constant = LEtoBE(*constant);
		writeUint32(ptr_WriteFile, *constant);
		printf("hex out in ldr consts = %x\n", *constant);
		file_line++;
		free(constant);
	}

	printf("Finished Program Loop 2\n");

	fclose(ptr_SourceFile);
	fclose(ptr_WriteFile);

  destroyAllDictionaries();

	printf("Finished program\n");
	return EXIT_SUCCESS;
}
