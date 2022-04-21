#ifndef LLIST_H_
#define LLIST_H_

#include <json-c/json.h>

/* VISITOR LINKED LIST (SORTED) IMPLEMENTATIONS */
// Insert a new visitor with name into the linked list
visitor_t* insert_visitor_ll(visitor_t** vis_db_head, char* name);
// Get a visitor from the linked list by name
visitor_t* get_visitor_ll(visitor_t** vis_db_head, char* name);
// Remove a visitor from the linked list by name
int remove_visitor_ll(visitor_t** vis_db_head, char* name);
// Print all visitors who visited a location
void print_location_visitors_ll(visitor_t** vis_db_head, int l_uid);
// Converts the linked list to a JSON Object
void to_json_ll(visitor_t** vis_db_head, json_object* dump);

#endif
