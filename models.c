#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "models.h"

// I'm using a tight growth stack to implement the storage of the visited locations

// Initialize the visitor struct
visitor_t* create_visitor(char* name) {
  visitor_t* new = (visitor_t *) malloc(sizeof(visitor_t));
  int* empty = (int *) malloc(sizeof(int) * BOUNDS);

  strcpy(new->name, name);
  new->locations = empty;
  new->top = -1; 
  new->len = BOUNDS; 

  // Linked List
  new->next = NULL;

  // BST
  new->left = NULL;
  new->right = NULL;
  new->height = 1;

  return new;
}

// Adds a visitor to the passed in location struct
void visitor_add_location(visitor_t* v, int l_uid) {
  if(v->top == v->len - 1) {
    // Extending the stack
    int* new = (int *) malloc(sizeof(int) * (v->len + BOUNDS));

    for (int i = 0; i < v->len; i++)
        new[i] = v->locations[i];

    v->len += BOUNDS;
    free(v->locations);
    v->locations = new;
  }

  v->locations[++(v->top)] = l_uid;
}

// Check if visistor visited location
int visitor_has_location(visitor_t* v, int l_uid) {
  for (int i = 0; i < v->len; i++) {
    if(v->locations[i] == l_uid) {
      return 1;
    }
  }

  return 0;
}
