#ifndef DICTIONARY_H
#define DICTIONARY_H


#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>

/* Define objects for dictionary objects */

typedef void KEY;
typedef void VALUE;

typedef struct treeNode {

  KEY *key; 
  VALUE *value;
  struct treeNode *left;
  struct treeNode *right;

} treeNode;

typedef struct dictionary{
  treeNode *tree;
} DICTIONARY;

/* Function definitions */

//Creates an Empty dictionary structure
DICTIONARY *createDictionary(void);

//Returns 1 if empty, 0 otherwise
int isEmpty(DICTIONARY *d);

//Returns 1 if put successful, 0 otherwise
int putElem(DICTIONARY *d , KEY *key , VALUE *value);

//Returns value at key in d
VALUE *getElem(DICTIONARY *d , KEY *key);

//Returns 1 if remove of key in d is successful, 0 otherwise
int removeElem(DICTIONARY *d , KEY *key);

//Deletes lowest node with lowest key and return value of that node
VALUE *removeLowestElem(DICTIONARY *d);

// returns number of elements in dictionary
uint32_t getNumElems(DICTIONARY *d);

//Returns 1 if all dictionary memory elements have been freed, 0 otherwise;
int destroyDictionary(DICTIONARY *d);

//Returns 1 if all dictionary memory elements have been freed, 0 otherwise;
int destroyDictionaryVALUES(DICTIONARY *d);

//Returns 1 if all dictionary memory elements have been freed, 0 otherwise;
int destroyDictionaryKEYS(DICTIONARY *d);

#endif
