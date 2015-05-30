#ifndef MEMORY_PROC_H
#define MEMORY_PROC_H

/* No need to include headers here -> they are included in the common_instances file*/
#include "common_instances.h"


#define MEM_SIZE 65536
/* Declaring the variable */

/* Possible error with this pointer TODO*/
extern int numBytes;

typedef struct{
	uint8_t *byte; 
}memory_machine;

extern memory_machine *memory;

/* Declare all function prototypes here */
uint32_t memory_swap_word_be_to_le(uint32_t word);
uint32_t memory_fetch_word(uint32_t pc);
void memory_write_word(uint16_t memory_index, uint32_t word_to_write);
void memory_machine_init(memory_machine **memptr);
void memory_machine_destroy(memory_machine *memptr);
void memory_cpu_init();
void memory_cpu_destroy();
void check_file_error(FILE *file);
void memory_load_file(FILE *file);

#endif
