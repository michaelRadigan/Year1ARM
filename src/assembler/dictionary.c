#include "dictionary.h"

/* 

typedef void KEY;
typedef void VALUE;

typedef struct dictionary_entry{
  
  KEY *key; 
  VALUE *value;

  dicionary_entry *next;

} ENTRY;

typedef struct dictionary{
 
  int size;
  ENTRY *root;

} DICTIONARY;

*/



/* Function definitions */


//Creates an Empty dictioary structure
DICTIONARY *createDictionary(void){
  //TODO
  return NULL;
}


//Returns 1 if empty, 0 otherwise
int isEmpty(DICTIONARY *d){
  //TODO
  return 0;
}

#include <stdio.h>
#include <stdint.h>
//Returns 1 if put successful, 0 otherwise
int putElem(DICTIONARY *d , KEY *key , VALUE *value){
  // printf("%s , %d" , (char *) key , *((int *) value) );
  //TODO
  return 0;
}


//Returns value at key in d
VALUE *getElem(DICTIONARY *d , KEY *key){
  //TODO
  return NULL;
}


//Returns 1 if remove of key in d is successful, 0 otherwise
int removeElem(DICTIONARY *d , KEY *key){
  //TODO
  return 0;
}

//Returns 1 if all dictioary memory elemnts have been freed, 0 otherwise;
int destroyDictionary(DICTIONARY *d){
  //TODO
  return 0;
}


