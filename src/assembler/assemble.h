#ifndef ASSEMBLE_H
#define ASSEMBLE_H

#include <stdlib.h>
#include <assert.h>
#include "dictionary.h"
#include "encode.h"

/* Structures */


DICTIONARY label_address;
DICTIONARY code_binarycode;
DICTIONARY opcode_function;


/* Function definitions */

DICTIONARY setUPlabel_address(void);
DICTIONARY setUPcode_binarycode(void);
DICTIONARY setUPopcode_function(void);

#endif
