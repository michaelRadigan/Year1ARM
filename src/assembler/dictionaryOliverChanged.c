#include "dictionary.h"

/*--------------------STRUCTURES FOR BST AND BST_ELEMS--------------------*/
/*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*/

/*
typedef void KEY;   
typedef void VALUE;  

typedef int (*bst_compare_t) (void *val1, void *val2);

struct bst_elem;

struct bst {
	bst_compare_t compare;   
	struct bst_elem *tree;
};

struct bst_elem {

	KEY *key; 
	VALUE *value;
	struct bst_elem *left;
	struct bst_elem *right;

};

*/

/*--------------------------AUXILIARY FUNCTIONS---------------------------*/
/*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*/

ENTRY *bst_alloc_elem(void) {
	ENTRY *elem = malloc(sizeof(ENTRY));
	if (elem == NULL) {
		perror("bst_alloc_elem");
		exit(EXIT_FAILURE);
	}
	return elem;
}

void dictionary_free_entry(ENTRY *elem) {
	free(elem);
}

void bst_init(DICTIONARY *handle, bst_compare_t compare) {
	handle->compare = compare;
	handle->tree = NULL;
}

void bst_insert(DICTIONARY *handle, void *value) {
	handle->tree = bst_insert_elem(handle->tree,handle->compare,value);
}

int string_compare(void *val1, void *val2) {
	return strcmp((const char*) val1, (const char*) val2);
}

ENTRY *bst_insert_elem(ENTRY *const elem,
		DICTIONARY_compare_t compare,KEY *key, VALUE *value) {
	if (elem==NULL) {
		struct bst_elem *new_elem =  bst_alloc_elem();
		new_elem->left = NULL;
		new_elem->right = NULL;
		new_elem->key = key;
		new_elem->value = value;
		return new_elem;
	} else {
		const int comparison  = compare(value, elem->value);
		if (comparison < 0 ) {
			elem->left =  bst_insert_elem(elem->left, compare, key, value);
		} else if (comparison > 0 ){
			elem->right = bst_insert_elem(elem->right,compare,key,value);        
		} 
		return elem;
	}
}

void bst_destroy_elem(ENTRY *elem) {
	if (elem == NULL)
		return;
	bst_destroy_elem(elem->left);
	bst_destroy_elem(elem->right);
	bst_free_elem(elem);
}


ENTRY getElemAux(ENTRY *d,
		DICTIONARY_compare_t compare, KEY *searchkey) {
	if (d->tree != NULL)	{
		void *keyptr = d->key;
		if (*keyptr == *searchkey) {
			return d->value;
		} else if (compare(searchkey, d->key) < 0){
			return getElemAux(d->tree->left, compare, searchkey); 
		} else {
			return getElemAux(d->tree->right, compare, searchkey);
		}
	} else {
		return NULL;
	}
}

/*
ENTRY deleteElemAux(ENTRY *d,
	 	bst_compare_t compare, KEY *searchkey) {

}
*/

/*----------------------------MAIN FUNCTIONS------------------------------*/
/*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*/

//Creates an Empty dictioary structure
DICTIONARY *createDictionary(void){
	DICTIONARY tree;
	bst_init(&tree, &string_compare)
}

//Returns 1 if empty, 0 otherwise
int isEmpty(DICTIONARY *d) {
	return d->tree == NULL;
}

#include <stdio.h>
#include <stdint.h>

//Returns value at key in d
VALUE *getElem(DICTIONARY *d , KEY *searchkey){
	if (d->tree != NULL)	{
		return getElemAux(d->tree, d->compare, searchkey)->value;
	} else {
		return NULL;
	}
}


//Returns 1 if put successful, 0 otherwise
void putElem(DICTIONARY *d , KEY *key , VALUE *value) {
	printf("KEY: %s , VALUE: %d" , (char *) key , *((int *) value));
	d->tree = bst_insert_elem(d->tree, d->compare, key, value);
}

//Returns 1 if remove of key in d is successful, 0 otherwise
/*
void removeElem(DICTIONARY *d , KEY *key){
   if ()	
	return 0;
}
*/
//Returns 1 if all dictioary memory elemnts have been freed, 0 otherwise;
void destroyDictionary(DICTIONARY *d){
	bst_destroy_elem(handle->tree);
}


