#ifndef MEMORY_PROC_H
#define MEMORY_PROC_H

/* No need to include headers here -> they are included in the common_instances file*/
#include "common_instances.h"

/* Declaring the variable */
extern int numBytes;

typedef struct{
	uint8_t *byte; 
}memory_machine;


/* Declare all function prototypes here */
uint32_t memory_fetch_word(uint32_t pc);
void memory_machine_init(memory_machine **memptr);
void memory_machine_destroy(memory_machine *memptr);
void memory_cpu_init(cpu *cpuptr);
void memory_cpu_destroy(cpu *cpuptr);
void check_file_error(FILE *file);
void memory_load_file(FILE *file);

#endif
