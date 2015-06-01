#include "dictionary.h"

/*------------AUX FUNCTIONS FOR PRINTING TREE PATHS--------------------------*/


/* Prototypes for funtions needed in printPaths() */
void printPathsRecur(treeNode *node, char path[], int pathLen);
void printArray(char ints[], int len);
 
/*Given a binary tree, print out all of its root-to-leaf
 paths, one per line. Uses a recursive helper to do the work.*/
void printPaths(treeNode *node) 
{
  char path[1000];
  printPathsRecur(node, path, 0);
}
 
/* Recursive helper function -- given a node, and an array containing
 the path from the root node up to but not including this node,
 print out all the root-leaf paths.*/
void printPathsRecur( treeNode* node, char path[], int pathLen) 
{
  if (node==NULL) 
    return;
 
  /* append this node to the path array */
  path[pathLen] = *(char *)node->key;
  pathLen++;
 
  /* it's a leaf, so print the path that led to here  */
  if (node->left==NULL && node->right==NULL) 
  {
    printArray(path, pathLen);
  }
  else
  {
    /* otherwise try both subtrees */
    printPathsRecur(node->left, path, pathLen);
    printPathsRecur(node->right, path, pathLen);
  }
}
 
 
/* UTILITY FUNCTIONS */
/* Utility that prints out an array on a line. */
void printArray(char ints[], int len) 
{
  int i;
  for (i=0; i<len; i++) 
  {
    printf("%c ", ints[i]);
  }
  printf("\n");
}    
 

/*---------------------AUXLIARY FUNCTIONS FOR AVL----------------------------*/

treeNode *rotateRight(treeNode *nodeN) {
  treeNode *nodeC = nodeN->left;
  nodeN->left = nodeC->right;
  nodeC->right = nodeN;
  return nodeC;
}

treeNode *rotateLeft(treeNode *nodeN) {
  treeNode *nodeC = nodeN->right;
  nodeN->right = nodeC->left;
  nodeC->left = nodeN;
  return nodeC;
}

treeNode *rotateLeftRight(treeNode *nodeN) {
  treeNode *nodeC = nodeN->left;
  treeNode *newLeft = rotateLeft(nodeC);
  nodeN->left = newLeft;
  return rotateRight(nodeN);
}

treeNode *rotateRightLeft(treeNode *nodeN) {
  treeNode *nodeC = nodeN->right;
  treeNode *newRight = rotateRight(nodeC);
  nodeN->right = newRight;
  return rotateLeft(nodeN);
}
//Creates an Empty dictionary structure
//DICTIONARY *createDictionary(void){
//  return NULL;
//}

int max(int a, int b) {
  if (a > b) {
    return a;
  } else {
    return b;
  }
}

int findHeight(treeNode *node) {
  if (node == NULL) {
      return -1;
  }
  return max(findHeight(node->left), findHeight(node->right))+1;
}

treeNode *reBalance(treeNode *nodeN) {
  if (findHeight(nodeN->left) - findHeight(nodeN-> right) > 1) {
    if (findHeight(nodeN->left->left) > findHeight(nodeN->left->right)) {
      nodeN = rotateRight(nodeN);
    }
    else {
     nodeN = rotateLeftRight(nodeN);
    }
  }
  else {
    if (findHeight(nodeN->right) - findHeight(nodeN-> left) > 1) {
      if (findHeight(nodeN->right->right) > findHeight(nodeN->right->left)) {
       nodeN = rotateLeft(nodeN);
      }
      else {
       nodeN = rotateRightLeft(nodeN);
      } /* else node is balanced  */
    }
  }
  return nodeN;
}

/*---------------------------------------------------------------------------*/

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

treeNode *insert(treeNode *node, KEY *key, VALUE *value) {
  if (node == NULL) {
    treeNode *temp;
    temp = malloc(sizeof(treeNode));
    temp->key = key;
    temp->value = value;
    temp->left = NULL;
    temp->right = NULL;
    return temp;
  }

  if (compare((void *)key, (void *)node->key) > 0) {
    node->right = insert(node->right, key, value);
    node = reBalance(node);
  } else if (compare((void *)key, (void *)node->key) < 0) {
    node->left = insert(node->left, key, value);
    node = reBalance(node);
  } 

  /* if reached here then value of a label/variable has changed */
  else {
    node->value = value;
  }
  return node;
}

treeNode *delete(treeNode *node, KEY *key) {
  if (node == NULL) {
    printf("Element not found\n");
    return (void *) -1;
  } else if (compare((void *)key, (void *)node->key) < 0){
    node->left = delete(node->left, key);
    node = reBalance(node);
  } else if (compare((void *)key, (void *)node->key) > 0){
    node->right = delete(node->right, key);
    node = reBalance(node);
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
      node = reBalance(node);
    }

  }
  return node;
}



VALUE *find(treeNode *node, KEY *key) {
  if (node == NULL) {   
    /* element not found */
    return 0;
  } 
  else if (compare((void *)key, (void *)node->key) > 0) {
    return find(node->right, key);
  } 
  else if (compare((void *)key, (void *)node->key) < 0) {
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
  printf(" %s: %d ",(char *)root->key, *(uint16_t *)root->value);  /*Print value */
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

int putElem(DICTIONARY *d , KEY *key , VALUE *value) {
  d->tree = insert(d->tree, key, value);
  return d->tree != NULL;
}

VALUE *getElem(DICTIONARY *d , KEY *key) {
  return find(d->tree, key); 
}


int removeElem(DICTIONARY *d , KEY *key) {
  treeNode *ptr = delete(d->tree, key);
  return ptr != (void *)-1;
}


int destroyDictionary(DICTIONARY *d) {
  return destroyENTRY(d->tree);
}

/*

int main() {
  uint16_t a = 1;
  uint16_t b = 2;
  uint16_t c = 3;
  uint16_t d = 4;
  uint16_t e = 5;
  uint16_t f = 6;
  uint16_t g = 7;
  uint16_t h = 8;
  uint16_t i = 9;
  uint16_t j = 10;
  uint16_t k = 11;
  uint16_t l = 12;
  uint16_t m = 0;

  DICTIONARY *dict = createDictionary();

  putElem(dict,"a",&a);
  putElem(dict,"b",&b);
  putElem(dict,"e",&e);
  putElem(dict,"c",&c);
  putElem(dict,"k",&k);
  putElem(dict,"d",&d);
  putElem(dict,"f",&f);
  putElem(dict,"j",&j);
  putElem(dict,"g",&g);
  putElem(dict,"h",&h);
  putElem(dict,"i",&i);
  putElem(dict,"l",&l);

  */

  /* test variable change */
/*  putElem(dict,"i", &m);

  printf("\ndeleted \"c\" and \"e\" \n\n");
  
  removeElem(dict,"c");  
  removeElem(dict, "e");     
  */

  /* Print Nodes in Inorder */
/*  printf("Inorder: \n");
  inorder(dict->tree);
  printf("\n");
  
  printf("\"a\" is : %x\n",*(uint16_t *)getElem(dict,"a"));
  printf("\"d\" is : %x\n",*(uint16_t *)getElem(dict,"d"));
  printf("\"b\" is : %x\n",*(uint16_t *)getElem(dict,"b"));

  

  printf("\n");
  printPaths(dict->tree);

  return 0;  
}  */

