#ifndef ENCODE_H
#define ENCODE_H

#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include "dictionary.h"
#include "encode.h"
#include <string.h>

/* Struct for storing function pointers */

  
/* Function defintions */
/* char *source is the string of words of the instructions */

/* Translate tests and, eor, sub, rsb, add ,orr instructions*/
void dp1_test(void);

/* Translates mov */
void dp2_test(void);

/* Translates tst, teq, cmp */
void dp3_test(void);

/* Translates mul */
void m_test(void);

/* Translates mla */
void ma_test(void);

/* Translates ldr */
void ldr_test(void);

/* Translates str */
void str_test(void);

/* Translates branch - beq, bne, bge, blt, bgt, ble, b, */
void b_test(void);

/* Translates special - andeq */
void andeq_test(void);

/* Translates special - lsl */
void lsl_test(void);


void run_encode_test(void);
#endif
