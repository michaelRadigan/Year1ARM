#include "dictionary.h"

/* 

typedef void key KEY;
typedef void value VALUE;

typedef struct dictionary_entry{
  
  KEY key  
  VALUE value

  ENTRY next

} ENTRY;

typedef struct dictionary{
 
  int size;
  ENTRY root;

} DICTIONARY;

*/



/* Function definitions */


//Creates an Empty dictioary structure
DICTIONARY create(void){

}


//Returns 1 if empty, 0 otherwise
int isEmpty(DICTIONARY d){

}


//Returns 1 if put successful, 0 otherwise
int put(DICTIONARY d , KEY key , VALUE value){

}


//Returns value at key in d
VALUE get(DICTIONARY d , KEY key){

}


//Returns 1 if remove of key in d is successful, 0 otherwise
int remove(DICTIONARY d , KEY key){

}

//Returns 1 if all dictioary memory elemnts have been freed, 0 otherwise;
int destry(DICTIONARY d){

}


