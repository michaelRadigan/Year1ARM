#include "common_instances.h"

struct Cpu cpu = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; /*Why the missing baraces error?*/

/*
typedef struct instructions{
  (*branchInstr)(uint32_t instr, cpu cpu);
  (*multInstr)(uint32_t instr , cpu cpu);
  (*dataProcInstr)(uint32_t instr , cpu cpu);
  (*sdtInstr)(uint32_t instr , cpu cpu);
}
*/



/**********************************************
*                  TODOS                      *
***********************************************
* Look into declaring a cpu in the header rather than passing it around like 
* Chuckle brothers style --  "To me, to you". <- <-!!!!
* Decode method
* setflags()
* nextfetch()
* multiply()
* etc
-------------------------------------------------------*/

/**********************************************
*        FUNCTION DELCARATIONS                *
**********************************************/

/**
* This will execute the next fetch
**/
uint32_t nextFetch();
void setFlags(uint32_t *instruction);







/**********************************************
*               SETUP METHODS                 *
**********************************************/



/**********************************************
*               DECODE METHODS                *
**********************************************/

/**
* Conditions Enum 
**/
typedef enum cond{ eq , ne , ge , lt , gt , le , al} Cond;


/**
*  @return 0 if condition is not met, 1 if met
**/ 
int checkCond(uint32_t instr){

  uint32_t condBinary = instr & 0xf0000000;
  condBinary >>= (2*7);
  Cond condition = (Cond) condBinary;

  uint32_t cpsr_ = cpu.cpsr;
  /* isolate the important bits */
  /*Reminder to ask Mickey why we need cpsr_*/
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

/**
*
*
**/
void branchInstri(uint32_t instr){
  
  if(checkCond(instr) == 0){
    // check if condition is satisfied.
    return;
  }
}  


void multiplyNonA(uint32_t *instruction){
    uint32_t rd, rm, rs;
    rm = (*instruction & 0x0000000F);
    rs = (*instruction & 0x00000F00) >> 8;
    rd = (*instruction & 0x000F0000) >> 16;
    //rd = rm * rs
    uint64_t a, b, c;
    c = 0;
    if(cpu.reg[rm] > cpu.reg[rs]){
        a = cpu.reg[rm];
        b = cpu.reg[rs];
    } else {
        a = cpu.reg[rs];
        b = cpu.reg[rm];  
    }

    while(b != 0){
        if((b & 1) != 0){
             c = c + a;
             a <<= 1;
             b >>= 1;
        }     
    }
    cpu.reg[rd] = (c & 0xFFFFFFFF); // check this!
}

/**
 * This is the helper function for multiply which carries
 * out both multiplucation and also addition. It updates teh values of the
 * registers using multiplyNonA as a helper function.
 * @Param instruction is a pointer to the instruction to be processed
 * @Param cpu is a (hopefully temporary) method of accessing the cpu
 * structure and updating the registers.*/ 

void multiplyAccumulate(uint32_t *instruction){
    multiplyNonA(instruction);
    int rn, rd;
    rn = (*instruction & 0x0000F000) >> 12;
    rd = (*instruction & 0x000F0000) >> 16;
    cpu.reg[rd] += cpu.reg[rn];
}

/**
 * This function carries out a multiply instruction
 * @param instuction is a 32bit instruction
 * @param cpu is is Cpu structure
*/
void multiply(uint32_t *instruction){
    if(checkCond(*instruction) == 0){
        return;
    }    
       /*Check these at some point*/
    if ((*instruction & 0x00200000) >> 20 == 1){
        multiplyAccumulate(instruction);    
    } else {
        multiplyNonA(instruction);
    }

    if ((*instruction & 0x00100000) >> 19 == 1){
        setFlags(instruction);
    }   
}
