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
  putElem(d,"eq",(void *) 0x0);
  putElem(d,"ne",(void *)0x1);
  putElem(d,"ge",(void *)0xa);
  putElem(d,"lt",(void *)0xb);
  putElem(d,"gt",(void *)0xc);
  putElem(d,"le",(void *)0xd);
  putElem(d,"al",(void *)0xe);

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

/* char *source is the string of words of the instructions */

/* Translates and, eor, sub, rsb, add ,orr instructions*/
uint32_t *dataProcessing1(char *source){
  //TODO
  return NULL;  
}

/* Translates mov */
uint32_t *dataProcessing2(char *source){
  //TODO
  return NULL;
}

/* Translates tst, teq, cmp */
uint32_t *dataProcessing3(char *source){
  //TODO
  return NULL;
}

/* Translates mul */
uint32_t *multiply(char *source){
  //TODO
  return NULL;
}

/* Translates mla */
uint32_t *multiplyAccum(char *source){
  //TODO
  return NULL;
}

/* Translates ldr */
uint32_t *sdt_ldr(char *source){
  //TODO
  return NULL;
}

/* Translates str */
uint32_t *sdt_str(char *source){
  //TODO
  return NULL;
}

/* Translates branch - beq, bne, bge, blt, bgt, ble, b, */
uint32_t *branch(char *source){
  //TODO
  return NULL;
}

/* Translates special - andeq */
uint32_t *spec_andeq(char *source){
  //TODO
  return NULL;
}

/* Translates special - lsl */
uint32_t *spec_lsl(char *source){
  //TODO
  return NULL;
}

