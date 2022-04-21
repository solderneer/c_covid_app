#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <json-c/json.h>

#include "models.h"
#include "bst.h"
#include "llist.h"

/* GLOBAL VISITOR HEAD */
visitor_t* vis_db_head = NULL;

/* GLOBAL VISITOR ROOT NODE */
visitor_t* vis_db_root = NULL;

int use_bst = 1; // IF 1 use BST, if 0 use Linked List (BST BY DEFAULT)

/* GENERICS WRAPPER */
void insert_visitor(char* name) {
  if(use_bst) {
    vis_db_root = insert_visitor_bst(vis_db_root, name);
  } else {
    insert_visitor_ll(&vis_db_head, name);
  }
}

visitor_t* get_visitor(char* name) {
  if(use_bst)
    return get_visitor_bst(vis_db_root, name);
  else {
    return get_visitor_ll(&vis_db_head, name);
  }
}

int remove_visitor(char* name) {
  if(use_bst) {
    vis_db_root = remove_visitor_bst(vis_db_root, name);
    return 0;
  } else {
    return remove_visitor_ll(&vis_db_head, name);
  }
}

// Utility function to simplify integer input checking
long getInteger() {
  char *p, s[100];
  long n = -1;

  while (fgets(s, sizeof(s), stdin)) {
      n = strtol(s, &p, 10);
      if (p == s || *p != '\n') {
        return -1;
      } else break;
  }

  return n;
}

void getTrimmedStr(char* buf, int buf_size) {
  fgets(buf, buf_size, stdin);
  buf = strtok(buf, "\r\n\t");
}

// Pretty prints out the visitor
void print_visitor(visitor_t* visitor) {
  printf("\n----------------------------\n");
  printf("VISITOR NAME: %s\n", visitor->name);
  printf("----------------------------\n");
}

// Pretty prints out the location
void print_location_visitors(l_uid) {
  printf("\n----------------------------\n");
  printf("LOCATION ID (%d)\n", l_uid);
  printf("Visitors:\n");
  
  if(use_bst)
    print_location_visitors_bst(vis_db_root, l_uid);
  else
    print_location_visitors_ll(&vis_db_head, l_uid);

  printf("----------------------------\n");
}

void db_to_file(char *db_file) {
  FILE *fp;
  json_object* dump = json_object_new_array();

  if(use_bst)
    to_json_bst(vis_db_root, dump);
  else
    to_json_ll(&vis_db_head, dump);
  
  fp = fopen(db_file,"w");
  fputs(json_object_to_json_string(dump), fp);
  fclose(fp);

  json_object_put(dump);
}

void file_to_db(char *db_file) {
  FILE *fp;
  char buffer[1024];
  
  fp = fopen(db_file,"r");
  fread(buffer, 1024, 1, fp);
	fclose(fp);

  json_object* parsed_json = json_tokener_parse(buffer);
  size_t n_visitors = json_object_array_length(parsed_json);
  printf("Importing %d visitors... \n", n_visitors);

  for(int i = 0; i < n_visitors; i++) {
    json_object* v = json_object_array_get_idx(parsed_json, i);

    json_object* name;
    json_object* locations;

    json_object_object_get_ex(v, "name", &name);
    json_object_object_get_ex(v, "locations", &locations);

    // Create the visitor
    insert_visitor(json_object_get_string(name));
    visitor_t* visitor = get_visitor(json_object_get_string(name));

    size_t n_locations = json_object_array_length(locations);
    for(int n = 0; n < n_locations; n++) {
      json_object* loc = json_object_array_get_idx(locations, n);
      visitor_add_location(visitor, json_object_get_int(loc));
    }
  }

  printf("Imported %d visitors successfully!\n", n_visitors);

}

/* MAIN FUNCTION */
int main(int argc, char *argv[]) {
  int option;
   // put ':' at the starting of the string so compiler can distinguish between '?' and ':'
  while((option = getopt(argc, argv, ":l")) != -1){ //get option from the getopt() method
    switch(option){
     //For option i, r, l, print that these are options
     case 'l':
      use_bst = 0;
      break;
     case ':':
      printf("Please specify a value\n");
      break;
     case '?': //used for some unknown options
      printf("unknown option: %c\n", optopt);
      break;
    }
  }

  if(use_bst)
    printf("Using a binary search tree (DEFAULT) use -l for a linked list!\n");
  else
    printf("Using a linked list\n");
  
  while(1) {
    printf("\nWelcome to the COVID Tracking App\n");
    printf("1) Create new visitor\n");
    printf("2) Remove visitor\n");
    printf("3) Add location visit\n");
    printf("4) View visitors to location\n");
    printf("5) Dump to file\n");
    printf("6) Load from file\n");
    printf("7) Quit\n\n");

    printf("Please pick an option: ");
    int n = getInteger();
    // Basic input validation
    while(n < 1 || n > 8) {
      printf("ERROR: Enter a valid integer: ");
      n = getInteger();
    }
    
    char name[50];
    int l_uid;

    switch(n) {
      case 1:
        printf("-> Provide new visitor details\n   Visitor Name: ");
        getTrimmedStr(name, 50);

        /* Check for duplicate */
        visitor_t* check = get_visitor(name);
        if(check != NULL) {
          printf("ERROR: Visitor %s already exists!\n", check->name);
          break;
        }

        insert_visitor(name);
        visitor_t* new = get_visitor(name); 

        printf("Created new visitor:");
        print_visitor(new);
        break;
      case 2:
        printf("-> Provide visitor details\n   Visitor Name: ");
        getTrimmedStr(name, 50);

        int status = remove_visitor(name);
        if(status == -1) {
          printf("ERROR: Visitor name does not exist!\n");
          break;
        }
          
        printf("Visitor %s deleted successfully!\n", name);
        break;
      case 3:
        printf("-> Add location visit\n   Visitor Name: ");
        getTrimmedStr(name, 50);

        // Retrieve and check if visitor exists
        visitor_t* v;
        v = get_visitor(name);
        if(v == NULL) {
          printf("ERROR: Visitor name does not exist!\n");
          break;
        }

        printf("   Location Id: ");
        l_uid = getInteger();
        // Basic input validation
        while(l_uid < 1) {
          printf("ERROR: Enter a valid positive integer: ");
          l_uid = getInteger();
        }

        // One-to-Many relationship
        visitor_add_location(v, l_uid);
          
        printf("\nAdded visit!");
        break;
      case 4:
        printf("-> View location visitors\n   Location Id: ");
        l_uid = getInteger();
        print_location_visitors(l_uid);
        break;
      case 5:
        printf("-> Dump to file\n   File Name: ");
        getTrimmedStr(name, 50);
        db_to_file(name);
        printf("Successfully dumped to %s", name);
        break;
      case 6:
        printf("-> Load from file\n   File Name: ");
        getTrimmedStr(name, 50);
        file_to_db(name);
        break;
      case 7:
        return 0;
      case 8:
        preorder(vis_db_root);
        break;
      default:
        printf("ERROR: Unexpected Error! Exiting...");
    }

    printf("\nPress enter to continue...");
    // Flushing buffer
    while ((getchar()) != '\n');
  }
}
