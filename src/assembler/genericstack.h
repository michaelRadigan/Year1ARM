#ifndef SRC_ASSEMBLER_GENERICSTACK_H_
#define SRC_ASSEMBLER_GENERICSTACK_H_

#include <stdlib.h>
#include <stdio.h>

typedef void ELEM;

typedef struct node{
	ELEM *elem;
	struct node *link;
} STACKELEM;

typedef struct stack {
	struct node *top;
//	int num;
}STACK;

/* Function protoypes */

STACK *createStack(void);
void destroyStack(STACK *ptr);
int isStackEmpty(STACK *ptr);
void push(STACK *ptr, ELEM *elem);
void pop(STACK *ptr);

#endif /* SRC_ASSEMBLER_GENERICSTACK_H_ */
