/**
 * @file threads_linked_list.h
 * @brief Header file for a linked list implementation to manage threads.
 * @author Mohamed Ezzat
 * @date 09/3/2025
 */

 #ifndef THREADS_LINKED_LIST_H
 #define THREADS_LINKED_LIST_H
 
 #include <pthread.h>
 
 #define EMPTY_PTHREAD 0  /**< Represents an empty pthread_t value. */
 
 /**
  * @brief Node structure for the linked list.
  * @details Each node contains a thread ID (`pthread_t`) and a pointer to the next node.
  */
 typedef struct Node {
     pthread_t data;          /**< Data of the node (pthread_t). */
     struct Node* next;       /**< Pointer to the next node. */
 } Node;
 
 /**
  * @brief LinkedList structure.
  * @details This structure represents a linked list with a head pointer.
  */
 typedef struct {
     Node* head;             /**< Head pointer for the linked list. */
 } LinkedList;
 
 /**
  * @brief Initializes a linked list.
  * @param list The linked list to initialize.
  */
 void init_list(LinkedList* list);
 
 /**
  * @brief Creates a new node with the given thread ID.
  * @param data The thread ID to store in the node.
  * @return Returns a pointer to the newly created node.
  */
 Node* create_node(pthread_t data);
 
 /**
  * @brief Adds a node at the end of the linked list.
  * @param list The linked list to which the node will be added.
  * @param data The thread ID to add to the list.
  */
 void add_node_end(LinkedList* list, pthread_t data);
 
 /**
  * @brief Deletes a node with the given thread ID from the linked list.
  * @param list The linked list from which the node will be deleted.
  * @param data The thread ID of the node to delete.
  */
 void delete_node(LinkedList* list, pthread_t data);
 
 /**
  * @brief Searches for a node with the given thread ID in the linked list.
  * @param list The linked list to search.
  * @param data The thread ID to search for.
  * @return Returns a pointer to the node if found, otherwise `NULL`.
  */
 Node* search_node(LinkedList* list, pthread_t data);
 
 /**
  * @brief Prints the contents of the linked list.
  * @param list The linked list to print.
  */
 void print_list(LinkedList* list);
 
 /**
  * @brief Retrieves the thread ID of the first node in the linked list.
  * @param list The linked list to retrieve the first node from.
  * @return Returns the thread ID of the first node.
  */
 pthread_t get_first_node(LinkedList* list);
 
 #endif /* THREADS_LINKED_LIST_H */