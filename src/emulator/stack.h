#ifndef STACK_H
#define STACK_H
#include <stdlib.h>
/**
 * Stack datatype for converting Little Endian file
 * to Big Endian for instruction parsing convenience
 */
typedef uint8_t *stackByteElem;

typedef struct {
	stackByteElem *contents;
	int maxSize;
	int stackTop;
}stackByte;

/* Function protoypes */

void stack_init(stackByte *stack_ptr, int maxSize);
void stack_destroy(stackByte *stack_ptr);
int  stack_is_empty(stackByte *stack_ptr);
int  stack_is_full(stackByte *stack_ptr);
void stack_push(stackByte *stack_ptr, stackByteElem *element);
stackByteElem StackPop(stackByte *stack_ptr);


#endif 
