#ifndef EMULATE_H
#define EMULATE_H

#include "common_instances.h"

/*Functions included in emulate*/

uint32_t nextFetch();
void setFlags(uint32_t *instruction);
void setUpCucle();

// helpers
int checkCond(uint32_t *instruction);

//instructions
void branchInstr(uint32_t *instruction);
void multiplyNonA(uint32_t *instruction);
void multiplyAccumulate(uint32_t *instruction);
void multiply(uint32_t *instruction);



#endif
