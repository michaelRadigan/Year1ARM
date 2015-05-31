#ifndef DICTIONARY_H
#define DICTIONARY_H


#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

/* Define objects for dictionary objects */

typedef void KEY;
typedef void VALUE;

typedef int (*DICTIONARY_compare_t) (void *val1, void *val2);

struct dictionary_entry{
  
  KEY *key; 
  VALUE *value;
	struct dictionary_entry *left;
	struct dictionary_entry *right;

};

typedef struct dictionary{
    DICTIONARY_compare_t compare;
    struct dictionary_entry *tree;

} DICTIONARY;



/* Function definitions */

//Creates an Empty dictioary structure
DICTIONARY *createDictionary(void);

//Returns 1 if empty, 0 otherwise
int isEmpty(DICTIONARY *d);

//Returns 1 if put successful, 0 otherwise
int putElem(DICTIONARY *d , KEY *key , VALUE *value);

//Returns value at key in d
VALUE *getElem(DICTIONARY *d , KEY *key);

//Returns 1 if remove of key in d is successful, 0 otherwise
//int removeElem(DICTIONARY *d , KEY *key);

//Returns 1 if all dictioary memory elemnts have been freed, 0 otherwise;
int destroyDictionary(DICTIONARY *d);

#endif
