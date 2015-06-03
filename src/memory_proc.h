#ifndef MEMORY_PROC_H
#define MEMORY_PROC_H


#include "common_instances.h"


#define MEM_SIZE 65536
#define BYTES    1
#define UNDEFINED 0xFFFFFFFF


typedef struct{
	uint8_t *byte; 
}memory_machine;

extern memory_machine *memory;


/* Function Prototypes */


uint32_t memory_swap_word_be_to_le(uint32_t word);
uint32_t memory_fetch_word(uint32_t pc);
void memory_write_word(uint16_t memory_index, uint32_t word_to_write);
void memory_machine_init(memory_machine **memptr);
void memory_machine_destroy(memory_machine **memptr);
void memory_cpu_init(void);
void memory_cpu_destroy(void);
void memory_instr_data_proc_init(void);
void memory_instr_mult_init(void);
void memory_instr_single_data_transfer_init(void);
void memory_instr_branch_init(void);
void memory_instr_data_proc_destroy(void);
void memory_instr_mult_destroy(void);
void memory_instr_single_data_transfer_destroy(void);
void memory_instr_branch_destroy(void);
void memory_instr_flags_init(void);
void memory_instr_flags_destroy(void);
void memory_pipeline_init(void);
void memory_pipeline_destroy(void);


void check_file_error(FILE *file);
void memory_load_file(FILE *file);

#endif
