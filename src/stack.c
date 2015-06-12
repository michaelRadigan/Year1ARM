#include "stack.h"

/**
 * Sets up an empty stack
 * @param stackptr Pointer to stack
 * @param maxSize  Size of the stack
 */
void
StackInit(stackByte *stackptr, int maxSize){
	stackByteElem *newContents;
	newContents = (stackByteElem *)malloc(sizeof(stackByteElem) * maxSize);

	if(newContents == NULL){
		fprintf(stderr, "Insufficient memory to initialize stack.\n");
        exit(1);  /* Exit, returning error code. */
	}

	stackptr->contents = newContents;
    stackptr->maxSize = maxSize;
    stackptr->stackTop = -1;  /* i.e. empty */
}


/**
 * Cleans up the stack
 * @param stackptr Pointer to the stack
 */
void
StackDestroy(stackByte *stackptr){
    free(stackptr->contents);

    stackptr->contents = NULL;
    stackptr->maxSize = 0;
    stackptr->stackTop = -1;  /* i.e. empty */
}

/**
 * Tests if stack is empty
 * @param stackptr Pointer to the stack
 */
int
StackIsEmpty(stackByte *stackptr){
    return stackptr->stackTop < 0;
}


/**
 * Tests if stack is full
 * @param stackptr Pointer to the stack
 */
int 
StackIsFull(stackByte *stackptr){
    return stackptr->stackTop >= stackptr->maxSize - 1;
}


/**
 * Push pointer to a byte onto stack
 * @param stackptr Pointer to the stack
 * @param stackByteElem Pointer to elem to be pushed onto stack
 */
void
StackPush(stackByte *stackptr, stackByteElem *element){
    if(StackIsFull(stackptr)){
    	fprintf(stderr, "Can't push element on stack: stack is full.\n");
        exit(1);  /* Exit, returning error code. */
  	}

	/* Push element to stack; update top. */
  	stackptr->contents[++stackptr->stackTop] = *element;
}


/**
 * Pops the element off the stack
 * @param stackptr Pointer to the stack
 */
stackByteElem 
StackPop(stackByte *stackptr){
  if(StackIsEmpty(stackptr)){
    fprintf(stderr, "Can't pop element from stack: stack is empty.\n");
    exit(1);  /* Exit, returning error code. */
  }

  return stackptr->contents[stackptr->stackTop--];
}


