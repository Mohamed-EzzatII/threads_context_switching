/**
 * @file threads_linked_list.c
 * @brief Implementation of a linked list for managing threads.
 * @author Mohamed Ezzat
 * @date 09/3/2025
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <pthread.h>
 #include "threads_linked_list.h"
 
 /**
  * @brief Creates a new node with the given thread ID.
  * @param data The thread ID to store in the node.
  * @return Returns a pointer to the newly created node.
  * @details This function allocates memory for a new node and initializes it with the given thread ID.
  */
 Node* create_node(pthread_t data)
 {
     Node* new_node = (Node*)malloc(sizeof(Node));
     if (new_node == NULL)
     {
         printf("Memory allocation failed!\n");
         exit(1);
     }
     new_node->data = data;
     new_node->next = NULL;
     return new_node;
 }
 
 /**
  * @brief Initializes a linked list.
  * @param list The linked list to initialize.
  * @details This function sets the head of the linked list to `NULL`.
  */
 void init_list(LinkedList* list)
 {
     list->head = NULL;
 }
 
 /**
  * @brief Adds a node at the end of the linked list.
  * @param list The linked list to which the node will be added.
  * @param data The thread ID to add to the list.
  * @details This function adds a new node with the given thread ID to the end of the list.
  *          If a node with the same thread ID already exists, the function does nothing.
  */
 void add_node_end(LinkedList* list, pthread_t data)
 {
     Node* new_node = create_node(data);
 
     if (list->head == NULL)
     {
         /* If the list is empty, make the new node the head */
         list->head = new_node;
     }
     else
     {
         /* Traverse to the end of the list */
         Node* temp = list->head;
         while (temp->next != NULL)
         {
             /* If the node exists, return and don't add it */
             if (pthread_equal(temp->data, data))
             {
                 return;
             }
             temp = temp->next;
         }
         /* Add the new node at the end if the node doesn't exist */
         temp->next = new_node;
     }
 }
 
 /**
  * @brief Deletes a node with the given thread ID from the linked list.
  * @param list The linked list from which the node will be deleted.
  * @param data The thread ID of the node to delete.
  * @details This function searches for a node with the given thread ID and deletes it from the list.
  */
 void delete_node(LinkedList* list, pthread_t data)
 {
     Node* temp = list->head;
     Node* prev = NULL;
 
     /* Traverse the list to find the node to delete */
     while (temp != NULL)
     {
         if (pthread_equal(temp->data, data))
         {
             /* Node found, delete it */
             if (prev == NULL)
             {
                 /* If the node to delete is the head */
                 list->head = temp->next;
             }
             else
             {
                 /* If the node to delete is in the middle or end */
                 prev->next = temp->next;
             }
             free(temp); /* Free the memory of the deleted node */
             return;
         }
         prev = temp;
         temp = temp->next;
     }
 }
 
 /**
  * @brief Searches for a node with the given thread ID in the linked list.
  * @param list The linked list to search.
  * @param data The thread ID to search for.
  * @return Returns a pointer to the node if found, otherwise `NULL`.
  */
 Node* search_node(LinkedList* list, pthread_t data)
 {
     Node* temp = list->head;
 
     /* Traverse the list to find the node */
     while (temp != NULL)
     {
         if (pthread_equal(temp->data, data))
         {
             /* Node found */
             return temp;
         }
         temp = temp->next;
     }
 
     /* If the node was not found */
     return NULL;
 }
 
 /**
  * @brief Retrieves the thread ID of the first node in the linked list.
  * @param list The linked list to retrieve the first node from.
  * @return Returns the thread ID of the first node.
  */
 pthread_t get_first_node(LinkedList* list)
 {
     return list->head->data;
 }
 
 /**
  * @brief Prints the contents of the linked list.
  * @param list The linked list to print.
  * @details This function prints the thread IDs stored in the linked list.
  */
 void print_list(LinkedList* list)
 {
     Node* temp = list->head;
     printf("Linked List: ");
     while (temp != NULL)
     {
         printf("%lu -> ", (unsigned long)temp->data);
         temp = temp->next;
     }
     printf("NULL\n");
 }