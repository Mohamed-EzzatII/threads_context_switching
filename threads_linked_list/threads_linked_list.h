#include <pthread.h>

#define EMPTY_PTHREAD 0

/* Define the Node structure */
typedef struct Node {
    pthread_t data;          /* Data of the node (pthread_t) */
    struct Node* next;       /* Pointer to the next node */
} Node;

/* Define the LinkedList structure */
typedef struct {
    Node* head;             /* Head pointer for the linked list */
} LinkedList;

/* Function to initialize a linked list */
void init_list(LinkedList* list);

/* Function to create a new node */
Node* create_node(pthread_t data) ;

/* Function to add a node at the end of the linked list */
void add_node_end(LinkedList* list,pthread_t data) ;

/* Function to delete a node by its data (pthread_t) */
void delete_node(LinkedList* list,pthread_t data) ;

/* Function to search for a node by its data (pthread_t) */
Node* search_node(LinkedList* list,pthread_t data) ;

/* Function to print the linked list */
void print_list(LinkedList* list);

pthread_t get_first_node(LinkedList* list);