#ifndef MODELS_H_
#define MODELS_H_

// How big the array should initially be and grow
#define BOUNDS 1000

typedef struct visitor_t visitor_t;

struct visitor_t {
  char name[50];
  
  // Visited locations tight growth stack
  int* locations;
  int top;
  int len;

  // Linked List
  visitor_t* next;

  // AVL BST
  visitor_t* left;
  visitor_t* right;
  int height;
};

/* MODEL FUNCTION PROTOTYPES */

/* ViSITOR FUNCTIONS */
// Initialize a visitor struct
visitor_t* create_visitor(char name[50]);

// Adds a location to the passed in visitor struct
void visitor_add_location(visitor_t* v, int l_uid);
// Checks if visitor visited location
int visitor_has_location(visitor_t* v, int l_uid);

#endif
