#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <json-c/json.h>

#include "models.h"
#include "bst.h"

/* AVL BINARY SEARCH TREE IMPLEMENTATION */
// Returns the height of the node
int height(visitor_t* v) {
  if(v == NULL)
    return 0;

  return v->height;
}

// Simple utility function to return the greater integer
int max(int a, int b) {
  return (a > b)? a : b;
}

// Prints out the tree using depth first traversal
void preorder(visitor_t* v) {
  if(v != NULL) {
    printf("%s", v->name);
    printf("traversing left\n");
    preorder(v->left);
    printf("traversing right\n");
    preorder(v->right);
  } else {
    printf("NULL\n");
  }
}

// Conducts a right rotation of the tree
visitor_t* right_rotate(visitor_t* v) {
  visitor_t* v1 = v->left;
  visitor_t* v2 = v1->right;

  v1->right = v;
  v->left = v2;

  v->height = max(height(v->left), height(v->right)) + 1;
  v1->height = max(height(v1->left), height(v1->right)) + 1;

  return v1;
}

// Conducts a left rotation of the tree
visitor_t* left_rotate(visitor_t* v) {
  visitor_t* v1 = v->right;
  visitor_t* v2 = v1->left;

  v1->left = v;
  v->right = v2;

  v->height = max(height(v->left), height(v->right)) + 1;
  v1->height = max(height(v1->left), height(v1->right)) + 1;

  return v1;
}

// Returns the difference between the number of children on the left and right
int get_balance(visitor_t* v) {
  if(v == NULL)
    return 0;

  int left = height(v->left);
  int right = height(v->right);

  return left - right;
}

// Inserts a new visitor into the tree
visitor_t* insert_visitor_bst(visitor_t* v, char* name) {
  if(v == NULL)
    return create_visitor(name);

  // Normal BST insertion
  if(strcmp(name, v->name) < 0)
    v->left = insert_visitor_bst(v->left, name);
  else if(strcmp(name, v->name) > 0)
    v->right = insert_visitor_bst(v->right, name);
  else
    return v;

  // Update ancestor height
  v->height = max(height(v->left), height(v->right)) + 1;

  // Get balance
  int balance = get_balance(v);
  
  // REBALANCING
  
  if(balance > 1 && strcmp(name, v->left->name) < 0)
    return right_rotate(v);

  if(balance < -1 && strcmp(name, v->right->name) > 0)
    return left_rotate(v);

  if(balance > 1 && strcmp(name, v->left->name) > 0) {
    v->left = left_rotate(v->left);
    return right_rotate(v);
  }

  if(balance < -1 && strcmp(name, v->right->name) < 0) {
    v->right = right_rotate(v->right);
    return left_rotate(v);
  }

  return v;
}

// Get the minimum value amidst the children
visitor_t* min_value(visitor_t* v) {
  visitor_t* temp = v;
  while(temp->left != NULL)
    temp = temp->left;

  return temp;
}

visitor_t* remove_visitor_bst(visitor_t* v, char* name) {
  if(v == NULL)
    return v;

  if(strcmp(name, v->name) < 0)
    v->left = remove_visitor_bst(v->left, name);
  else if(strcmp(name, v->name) > 0)
    v->right = remove_visitor_bst(v->right, name);
  else {
    // Found the correct node
    if(v->left == NULL || v->right == NULL) {
      visitor_t* temp = v->left ? v->left : v->right;
      
      if(temp == NULL) {
        temp = v;
        v = NULL;
      } else {
        *v = *temp;
      }

      free(temp);
    } else {
      visitor_t* temp = min_value(v->right);
      
      // Copy the data
      strcpy(v->name, temp->name);
      v->locations = temp->locations;
      v->top = temp->top;
      v->len = temp->len;

      v->right = remove_visitor_bst(v->right, temp->name);      
    }
  }

  if(v == NULL)
    return v;

  v->height = max(height(v->left), height(v->right)) + 1;
  int balance = get_balance(v);

  // REBALANCING

  if(balance > 1 && get_balance(v->left) >= 0)
    return right_rotate(v);

  if(balance > 1 && get_balance(v->left) < 0) {
    v->left = left_rotate(v->left);
    return right_rotate(v);
  }

  if(balance < -1 && get_balance(v->right) <= 0)
    return left_rotate(v);

  if(balance < -1 && get_balance(v->right) > 0) {
    v->right = right_rotate(v->right);
    return left_rotate(v);
  }

  return v;
}

// Recursive get operation from the tree
visitor_t* get_visitor_bst(visitor_t* v, char* name) {
  // Base case
  if(v == NULL || strcmp(name, v->name) == 0)
    return v;

  if(strcmp(name, v->name) < 0)
    return get_visitor_bst(v->left, name);
  else
    return get_visitor_bst(v->right, name);
}

// Recursively searches all the leaves to check if they have visited a location
void print_location_visitors_bst(visitor_t* v, int l_uid) {
  if(v != NULL) {
    if(visitor_has_location(v, l_uid))
      printf("%s\n", v->name);
    
    // Recurse down to all children
    print_location_visitors_bst(v->left, l_uid);
    print_location_visitors_bst(v->right, l_uid);
  }
}

// Dumps the tree to a standard JSON format
void to_json_bst(visitor_t* v, json_object* dump) {
  if(v != NULL) {
    json_object* dump_visitor = json_object_new_object();

    // Add name of visitor
    json_object* current_name = json_object_new_string(v->name);
    json_object_object_add(dump_visitor, "name", current_name);

    // Create visited locations array
    json_object* current_locations = json_object_new_array_ext((v->top) + 1);
    for(int i = 0; i < (v->top + 1); i++) {
      json_object* l_uid = json_object_new_int(v->locations[i]);
      json_object_array_add(current_locations, l_uid);
    }
    json_object_object_add(dump_visitor, "locations", current_locations);

    // Add visitor to db
    json_object_array_add(dump, dump_visitor);
    
    // Recurse down to all children
    to_json_bst(v->left, dump);
    to_json_bst(v->right, dump);
  }
}
