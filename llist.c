#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <json-c/json.h>

#include "models.h"
#include "llist.h"

/* LINKED LIST IMPLEMENTATION */
// Adds a new visitor to the db
visitor_t* insert_visitor_ll(visitor_t** vis_db_head, char* name) {
  visitor_t* new = create_visitor(name);

  // Empty DB, just insert to head
  if(*vis_db_head == NULL) {
    *vis_db_head = new;
    return new;
  }

  // Ordered insert by alphabetical order
  visitor_t* prev = NULL;
  visitor_t* current = *vis_db_head;

  while(strcmp(new->name, current->name) > 0) {
    prev = current;
    current = current->next;

    // Tail of the list
    if(current == NULL)
      break;
  }

  // Avoid duplicate names
  if(current != NULL) {
    if(strcmp(name, current->name) == 0) {
      return NULL;
    }
  }

  // Inserting at head
  if(prev == NULL) {
    new->next = *vis_db_head;
    *vis_db_head = new;
    
    return new;
  }
  
  prev->next = new;
  new->next = current;

  return new;
}

// Searches the list for a visitor by name
visitor_t* get_visitor_ll(visitor_t** vis_db_head, char* name) {
  visitor_t* prev = NULL;
  visitor_t* current = *vis_db_head;

  // Check for empty head
  if(current == NULL)
    return NULL;
  
  while(strcmp(name, current->name) > 0) {
    prev = current;
    current = current->next;

    // Tail of the list, visitor not found
    if(current == NULL) {
      return NULL;
    }
  }
  
  if(strcmp(name, current->name) == 0) {
    // Found
    return current;
  } else {
    // Visitor not found
    return NULL;
  }
}

// Finds and then removes a visitor from the list
int remove_visitor_ll(visitor_t** vis_db_head, char* name) {
  visitor_t* prev = NULL;
  visitor_t* current = *vis_db_head;
  
  // Check for empty head
  if(current == NULL)
    return -1;
  
  while(strcmp(name, current->name) > 0) {
    prev = current;
    current = current->next;

    // Tail of the list, visitor not found
    if(current == NULL) {
      return -1;
    }
  }

  if(strcmp(name, current->name) == 0) {
    // FOUND IT!
    if((current->next == NULL) && (prev == NULL)) {
      // Only element left
      *vis_db_head = NULL;
    } else if(prev == NULL) {
      // Removing at head
      *vis_db_head = current->next;
    } else if (current->next == NULL){
      // Removing at tail
      prev->next = NULL;
    } else {
      // Removing in the middle
      prev->next = current->next;
    }

    free(current);
  } else {
    // Visitor not found
    return -1;
  }
}

// Traverses the whole list and prints out everyone who has visited a location
void print_location_visitors_ll(visitor_t** vis_db_head, int l_uid) {
  visitor_t* current = *vis_db_head;

  // Traverse the linked list
  while(current != NULL) {
    if(visitor_has_location(current, l_uid))
      printf("%s\n", current->name);
    current = current->next;
  } 
}

// Dumps out the list to a standard JSON format
void to_json_ll(visitor_t** vis_db_head, json_object* dump) {
  visitor_t* current = *vis_db_head;

  while(current != NULL) {
    json_object* dump_visitor = json_object_new_object();

    // Add name of visitor
    json_object* current_name = json_object_new_string(current->name);
    json_object_object_add(dump_visitor, "name", current_name);

    // Create visited locations array
    json_object* current_locations = json_object_new_array_ext((current->top) + 1);
    for(int i = 0; i < (current->top + 1); i++) {
      json_object* l_uid = json_object_new_int(current->locations[i]);
      json_object_array_add(current_locations, l_uid);
    }
    json_object_object_add(dump_visitor, "locations", current_locations);

    // Add visitor to db
    json_object_array_add(dump, dump_visitor);

    current = current->next;
  }

  return dump;
}
