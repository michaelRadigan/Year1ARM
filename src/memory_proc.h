#ifndef MEMORY_PROC_H
#define MEMORY_PROC_H

/* No need to include headers here -> they are included in the common_instances file*/
#include "common_instances.h"

/* Declaring the variable */
extern int numBytes;

typedef struct memory_machine{
	uint8_t *byte; 
}memory_machine;


/* Declare all function prototypes here */
void memory_load_file(FILE *file);

#endif
