#ifndef ASSEMBLE_H
#define ASSEMBLE_H

#include <stdlib.h>
#include <assert.h>
#include <strings.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"
#include "encode.h"

/* Structures */


DICTIONARY *label_address;
DICTIONARY *code_binarycode;
DICTIONARY *opcode_function;


/* Function definitions */

DICTIONARY *setUPlabel_address(void);
DICTIONARY *setUPcode_binarycode(void);
DICTIONARY *setUPopcode_function(void);
void destroyDictionaryfunctions(DICTIONARY *d);
char *getLabel(char *source);
void writeBits(uint32_t *bits,FILE *out_file);
#endif
