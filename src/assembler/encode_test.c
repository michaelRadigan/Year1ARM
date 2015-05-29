#include "encode_test.h"

  





/* Translate tests and, eor, sub, rsb, add ,orr instructions*/
void dp1_test(void){
  const char test_1[] = "add r2, r5";
  const char test_2[] = "eor r8, r10";
  const char test_3[] = "sub r1, r2";

  const uint32_t awns_1;

}

/* Translates mov */
void dp2_test(void){


}

/* Translates tst, teq, cmp */
void dp3_test(void){


}

/* Translates mul */
void m_test(void){



}

/* Translates mla */
void ma_test(void){


}

/* Translates ldr */
void ldr_test(void){



}

/* Translates str */
void str_test(void){



}

/* Translates branch - beq, bne, bge, blt, bgt, ble, b, */
void b_test(void){



}

/* Translates special - andeq */
void andeq_test(void){



}

/* Translates special - lsl */
void lsl_test(void){



}

void run_encode_test(void){
  dp1_test();
  dp2_test();
  dp3_test();
  m_test();
  ma_test();
  lsr_test();
  str_test();
  b_test();
  andeq_test();
  lsl_Test();
  printf("All encode_tests run");
}


