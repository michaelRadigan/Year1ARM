#include "dictionary.h"

/*------------AUX FUNCTIONS FOR PRINTING TREE PATHS--------------------------*/


/* Prototypes for funtions needed in printPaths() */
void printPathsRecur(treeNode *node, int path[], int pathLen);
void printArray(int ints[], int len);
 
/*Given a binary tree, print out all of its root-to-leaf
 paths, one per line. Uses a recursive helper to do the work.*/
void printPaths(treeNode *node) 
{
  int path[1000];
  printPathsRecur(node, path, 0);
}
 
/* Recursive helper function -- given a node, and an array containing
 the path from the root node up to but not including this node,
 print out all the root-leaf paths.*/
void printPathsRecur( treeNode* node, int path[], int pathLen) 
{
  if (node==NULL) 
    return;
 
  /* append this node to the path array */
  path[pathLen] = node->value;
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
void printArray(int ints[], int len) 
{
  int i;
  for (i=0; i<len; i++) 
  {
    printf("%d ", ints[i]);
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

treeNode *insert(treeNode *node, KEY key, VALUE value) {
  if (node == NULL) {
    treeNode *temp;
    temp = malloc(sizeof(treeNode));
    temp->key = key;
    temp->value = value;
    temp->left = NULL;
    temp->right = NULL;
    return temp;
  }

  if (compare(key, node->key) > 0) {
    node->right = insert(node->right, key, value);
    node = reBalance(node);
  } else if (compare(key, node->key) < 0) {
    node->left = insert(node->left, key, value);
    node = reBalance(node);
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
    node = reBalance(node);
  } else if (compare(key, node->key) > 0){
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


int main() {
  /*Code To Test the logic
    Creating an example tree
     5
    / \
   3  10
  / \   \
 1   4   11
    */
  DICTIONARY *dict = createDictionary();
  
  putElem(dict,"a",1);
  putElem(dict,"b",2);
  putElem(dict,"c",3);
  putElem(dict,"d",4);
  putElem(dict,"e",5);
  putElem(dict,"f",6);
  putElem(dict,"g",7);
  putElem(dict,"h",8);
  putElem(dict,"i",9);
  putElem(dict,"j",10);
  putElem(dict,"k",11);
  putElem(dict,"l",12);

  /* test variable change */
  putElem(dict,"i", 0);

  removeElem(dict,"c");  
  removeElem(dict, "e");   

  /* Print Nodes in Inorder */
  printf("Inorder: ");
  inorder(dict->tree);
  printf("\n");
  
  printf("\"a\" is : %x\n",getElem(dict,"a"));
  printf("\"d\" is : %x\n",getElem(dict,"d"));
  printf("\"b\" is : %x\n",getElem(dict,"b"));

  printf("\n");
  printPaths(dict->tree);

  return 0;  
}  

