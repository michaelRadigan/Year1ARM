#ifndef ENCODE_H
#define ENCODE_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "dictionary.h"

//For Access to label_address Dictionary
#include "assemble.h"

//For access to cpu registers enum
#include "../emulator/cpu.h"


DICTIONARY *register_dict;
DICTIONARY *LDRconsts;

/* Struct for storing function pointers */


typedef uint32_t *(*Encode)(char *);

typedef struct storeEncode{
    Encode encFunc;
}STR_ENC;

DICTIONARY *code_binarycode;

/* The structs required to store the functions */
STR_ENC *dp1;
STR_ENC *dp2;
STR_ENC *dp3;
STR_ENC *m;
STR_ENC *ma;
STR_ENC *ldr;
STR_ENC *str;
STR_ENC *b;
STR_ENC *andeq;
STR_ENC *lsl;
  
/* Function defintions */
void setUPregister_dict(void);

void destroyRegisterDictionary(void);

void setUPcode_binarycode(void);

void destroycode_binarycode(void);

void setUpLDRconsts(void);

void destroyLDRconsts(void);

/* Auxiliary functions defintions*/


uint32_t *binaryConcat( uint32_t *b1, uint32_t *b2 , int pos);

uint32_t *binaryReplace( uint32_t *b1 , int numberOfBits, uint32_t *b2 , int pos);

/* Setsup structs containing function pointers */
void setUPFuncStructs(void);

/* Frees all funcStructs */
void destroyFuncStructs(void);

/* char *source is the string of words of the instructions */

/* Translates and, eor, sub, rsb, add ,orr instructions*/
uint32_t *dataProcessing1(char *source);

/* Translates mov */
uint32_t *dataProcessing2(char *source); 

/* Translates tst, teq, cmp */
uint32_t *dataProcessing3(char *source);

/* Translates mul */
uint32_t *multiply(char *source);

/* Translates mla */
uint32_t *multiplyAccum(char *source);

/* Translates ldr */
uint32_t *sdt_ldr(char *source);

/* Translates str */
uint32_t *sdt_str(char *source);

/* Translates branch - beq, bne, bge, blt, bgt, ble, b, */
uint32_t *branch(char *source);

/* Translates special - andeq */
uint32_t *spec_andeq(char *source);

/* Translates special - lsl */
uint32_t *spec_lsl(char *source);

#endif
