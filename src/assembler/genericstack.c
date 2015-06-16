#include "genericstack.h"

STACK *createStack(void) {
	STACK *res = malloc(sizeof(STACK *));
	res->top = NULL;
//	res->num = 0;
	return res;
}

void push(STACK *ptr, ELEM *elem) {
	STACKELEM *temp = malloc(sizeof(STACKELEM *));
	temp->elem = elem;
	temp->link = ptr->top;
	ptr->top = temp;
//	ptr->num++;
}

void pop(STACK *ptr) {
	STACKELEM *temp;
	if (ptr->top == NULL) {
		return;
	}
	temp = ptr->top;
	ptr->top = ptr->top->link;
	free(temp);
//	if (ptr->num > 0) {
//		ptr->num--;
//	}
}

void destroyStack(STACK *ptr) {
	free(ptr);
}

int isStackEmpty(STACK *ptr) {
	if (ptr == NULL) {
		perror("Stack has not been initialised");
	}
	return ptr->top == NULL;
}

void printStack(STACK *ptr) {

}




