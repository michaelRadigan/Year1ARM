#include "common_instances.h"

typedef struct cpu{
  uint32_t *register[14] 
  uint32_t *pc;
  uint32_t *cpsr;
  uint32_t decode;
  uint32_t encode;
}cpu;






