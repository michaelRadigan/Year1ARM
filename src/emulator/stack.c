#include "stack.h"

/**
 * Sets up an empty stack
 * @param stackptr Pointer to stack
 * @param maxSize  Size of the stack
 */
void
stack_init(stackByte *stack_ptr, int maxSize){
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
StackDestroy(stackByte *stack_ptr){
  free(stackptr->contents);

  stack_ptr->contents = NULL;
  stack_ptr->maxSize = 0;
  stack_ptr->stackTop = -1;  /* i.e. empty */
}

/**
 * Tests if stack is empty
 * @param stackptr Pointer to the stack
 */
int
stack_is_empty(stackByte *stack_ptr){
  return stack_ptr->stackTop < 0;
}


/**
 * Tests if stack is full
 * @param stackptr Pointer to the stack
 */
int 
stack_is_full(stackByte *stack_ptr){
  return stack_ptr->stackTop >= stack_ptr->maxSize - 1;
}


/**
 * Push pointer to a byte onto stack
 * @param stackptr Pointer to the stack
 * @param stackByteElem Pointer to elem to be pushed onto stack
 */
void
stack_push(stackByte *stack_ptr, stackByteElem *element){
  if(StackIsFull(stackptr)){
    fprintf(stderr, "Can't push element on stack: stack is full.\n");
    exit(1);  /* Exit, returning error code. */
  }
        /* Push element to stack; update top. */
  stack_ptr->contents[++stack_ptr->stackTop] = *element;
}


/**
 * Pops the element off the stack
 * @param stackptr Pointer to the stack
 */
stackByteElem 
StackPop(stackByte *stack_ptr){
  if(StackIsEmpty(stack_ptr)){
    fprintf(stderr, "Can't pop element from stack: stack is empty.\n");
    exit(1);  /* Exit, returning error code. */
  }

  return stack_ptr->contents[stack_ptr->stackTop--];
}


