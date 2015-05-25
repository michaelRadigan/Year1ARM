#include "common_instances.h"

typedef struct cpu{
  uint32_t *reg[14]; 
  uint32_t *pc;
  uint32_t *cpsr;
  uint32_t decode;
  uint32_t encode;
}Cpu;

/*
typedef struct instructions{
  (*branchInstr)(uint32_t instr, cpu cpu);
  (*multInstr)(uint32_t instr , cpu cpu);
  (*dataProcInstr)(uint32_t instr , cpu cpu);
  (*sdtInstr)(uint32_t instr , cpu cpu);
}
*/

/**
  
**/
typedef enum cond{ eq , ne , ge , lt , gt , le , al} Cond;


//this will execute the next fetch
uint32_t nextFetch();


void setUpCycle(){
    Cpu cpustruct;
    cpustruct.pc = 0;
    cpustruct.cpsr = 0;
    int i;
    for(i = 0; i< 14; i++){
        cpustruct.reg[i] = 0;
    }
    cpustruct.decode = nextFetch();
    cpustruct.encode = nextFetch();

}



/**
  @return 0 if condition is not met, 1 if met
**/  
int checkCond(Cond condition, Cpu cpu){

  uint32_t cpsr_ = *(cpu.cpsr);
  // isolate the important bits
  uint32_t v = cpsr_ & 0x10000000;
  uint32_t z = cpsr_ & 0x40000000;
  uint32_t n = cpsr_ & 0x80000000;

  switch(condition){
    case eq: 
      return n==1;
    case ne:
      return n==0;
    case ge:
      return n==v;
    case lt:
      return n!=v;
    case gt:
      return (z==0)&&(n==v);
    case le:
      return (z==1)||(n!=v);
    case al:
      return 1;
    default:
      return 0;
  }
}

void branchInstri(uint32_t instr, Cpu cpu ){

  uint32_t condBinary = instr & 0xf0000000;
  condBinary >>= (2*7);
  
  if(checkCond((Cond) condBinary , cpu) == 0){
    // check if condition is satisfied.
    return;
  }
}  











