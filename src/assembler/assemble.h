#ifndef ASSEMBLE_H
#define ASSEMBLE_H

#include <stdlib.h>
#include <assert.h>
#include <strings.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"
#include "encode.h"
#include <sys/stat.h>
#include <limits.h>

/* Structures */


DICTIONARY *label_address;
DICTIONARY *opcode_function;
DICTIONARY *alias_register;

uint32_t file_line;
uint32_t file_length;

/* Function definitions */
void setUpDictionaries();
DICTIONARY *setUPlabel_address(void);
DICTIONARY *setUpalias_register(void);
DICTIONARY *setUPopcode_function(void);
void destroyDictionaryfunctions(DICTIONARY *d);
void destroyAllDictionaries();
int storeLabel(char *source);
char *removeLabel(char *source);
int writeUint32(FILE * const stream, uint32_t value);
int doesFileExist(const char *filename);
void writeBits(uint32_t *bits,FILE *out_file);
uint32_t LEtoBE(uint32_t word);
#endif
