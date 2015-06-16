#ifndef STACK_H
#define STACK_H

#include "common_instances.h"

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

void StackInit(stackByte *stackptr, int maxSize);
void StackDestroy(stackByte *stackptr);
int  StackIsEmpty(stackByte *stackptr);
int  StackIsFull(stackByte *stackptr);
void StackPush(stackByte *stackptr, stackByteElem *element);
stackByteElem StackPop(stackByte *stackptr);


#endif 
