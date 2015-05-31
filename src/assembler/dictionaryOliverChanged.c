#include "dictionary.h"

/*-------------------------AUXLIARY FUNCTIONS--------------------------------*/


/* returns 1 if 1st arg > 2nd arg, -1 if vice versa and 0 if equal */
int compare(void *fst, void *snd) {
  return strcmp((const char*) fst, (const char*) snd); 
} 

/* key compatible */
treeNode *findMin(treeNode *node) {
  while (node->left != NULL) {
    node = node->left;
  }
  return node;
}

treeNode *insert(treeNode *node, KEY key, VALUE value) {
  if (node == NULL) {
    treeNode *temp;
    temp = malloc(sizeof(treeNode));
    temp->key = key;
    temp->value = value;
    temp->left = NULL;
    temp->right = NULL;
  printf("temp pointer in insert = %p\n", (void *) temp);
  return temp;
  }

  if (compare(key, node->key) > 0) {
    node->right = insert(node->right, key, value);
  } else if (compare(key, node->key) < 0) {
    node->left = insert(node->left, key, value);
  } 

  /* if reached here then value of a label/variable has changed */
  else {
    node->value = value;
  }
  return node;
}

treeNode *delete(treeNode *node, KEY key) {
  if (node == NULL) {
    printf("Element not found\n");
    return (void *) -1;
  } else if (compare(key, node->key) < 0){
    node->left = delete(node->left, key);
  } else if (compare(key, node->key) > 0){
    node->right = delete(node->right, key);
  } else {

    /* Case 1 : If no child */
    if (node->left == NULL && node->right == NULL) {
      free(node);
      node = NULL;
    }

    /* Case 2 : if one child */
    else if (node->left == NULL) {
      treeNode *temp = node;
      node = node->right;
      free(temp);
    } 
    else if (node->right == NULL) {
      treeNode *temp = node;
      node = node->left;
      free(temp);
    }

    /* Case 3 : two children */
    else {
      treeNode *temp = findMin(node->right);
      node->key = temp->key;
      node->value = temp->value;
      node->right = delete(node->right,temp->key);
    }

  }
  return node;
}

VALUE find(treeNode *node, KEY key) {
  if (node == NULL) {
    /* element not found */
    return 0;
  } 
  else if (compare(key, node->key) > 0) {
    return find(node->right, key);
  } 
  else if (compare(key, node->key) < 0) {
    return find(node->left, key);
  }  
  else {
    /* element found */
    return node->value;
  }
}

int destroyENTRY(treeNode *node) {
  if(node == NULL) {
    return 1;
  } 
  destroyENTRY(node->left);
  destroyENTRY(node->right);
  destroyENTRY(node);
  return 0;
}

void inorder(treeNode *root) {
  if(root == NULL) return;

  inorder(root->left);       /*Visit left subtree */
  printf(" %s: %d ",root->key, root->value);  /*Print value */
  inorder(root->right);      /* Visit right subtree */
}

/*---------------------------------------------------------------------------*/
/*----------------------------MAIN FUNCTIONS---------------------------------*/

DICTIONARY *createDictionary(void) {
  DICTIONARY *dictionary;
  dictionary = malloc(sizeof(DICTIONARY));
  dictionary->tree = NULL;
  return dictionary;
}

int isEmpty(DICTIONARY *d) {
 return d->tree == NULL;
}

int putElem(DICTIONARY *d , KEY key , VALUE value) {
  d->tree = insert(d->tree, key, value);
  printf("d->tree pointer in putElem after insert = %p\n", (void *) d->tree);
  return d->tree != NULL;
}

VALUE getElem(DICTIONARY *d , KEY key) {
  return find(d->tree, key); 
}

int removeElem(DICTIONARY *d , KEY key) {
  treeNode *ptr = delete(d->tree, key);
  return ptr != (void *)-1;
}

int destroyDictionary(DICTIONARY *d) {
  return destroyENTRY(d->tree);
}

/*
int main() {
  int a; */
  /*Code To Test the logic
    Creating an example tree
     5
    / \
   3  10
  / \   \
 1   4   11
    */
/*  DICTIONARY *dict = createDictionary();
  printf("dict pointer = %p\n", (void*) dict);
  printf("dict->tree pointer = %p\n", (void *) dict->tree);
  a = putElem(dict,"a",5);
  printf("dict->tree pointer = %p\n", (void *) dict->tree);


  a = putElem(dict,"b",10);
  a = putElem(dict,"c",3); a = putElem(dict,"d",4);
  a = putElem(dict,"e",1); a = putElem(dict,"f",11);
  a = putElem(dict, "b", 30);


  printf(" a = %d\n", a);   */
  /* Deleting node with value 5, change this value to test other cases */
/*  a = removeElem(dict,"c");
  a = removeElem(dict, "e");   */

  /* Print Nodes in Inorder */
/*  printf("Inorder: ");
  inorder(dict->tree);
  printf("\n");
  
  printf("\"a\" is : %x\n",getElem(dict,"a"));
  printf("\"d\" is : %x\n",getElem(dict,"d"));
  printf("\"b\" is : %x\n",getElem(dict,"b"));
  return 0;  
}  */

