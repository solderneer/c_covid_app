#ifndef BST_H_
#define BST_H_

#include <json-c/json.h>

/* VISITOR AVL BALANCED BINARY SEARCH TREE FUNCTIONS */
// Insert a new visitor with name into the BST
visitor_t* insert_visitor_bst(visitor_t* v, char* name);
// Remove a visitor with name from the BST
visitor_t* remove_visitor_bst(visitor_t* v, char* name);
// Get a visitor by name
visitor_t* get_visitor_bst(visitor_t* v, char* name);
// Print out the visitors to a location
void print_location_visitors_bst(visitor_t* v, int l_uid);
// Converts the AVL tree to a JSON Object
void to_json_bst(visitor_t* v, json_object* dump);

/* UTILITY DEBUG FUNTIONS */
// Simple unordered graph printout (depth first)
void preorder(visitor_t* v);


#endif
