#include "encode.h"


/* char *source is the string of words of the instructions */

/* Translates and, eor, sub, rsb, add ,orr instructions*/
uint32_t dataProcessing1(char *source){


}

/* Translates mov */
uint32_t dataProcessing2(char *source){



}

/* Translates tst, teq, cmp */
uint32_t dataProcessing3(char *source){




}

/* Translates mul */
uint32_t multiply(char *source){




}

/* Translates mla */
uint32_t multiplyAccum(char *source){



}

/* Translates ldr */
uint32_t sdt_ldr(char *source){



}

/* Translates str */
uint32_t sdt_str(char *source){



}

/* Translates branch - beq, bne, bge, blt, bgt, ble, b, */
uint32_t branch(char *source){




}

/* Translates special - andeq */
uint32_t spec_andeq(char *source){





}

/* Translates special - lsl */
uint32_t spec_lsl(char *source){





}

#endif
