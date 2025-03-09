#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include"threads_linked_list.h"

/* Function to create a new node */
Node* create_node(pthread_t data) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

/* Function to initialize a linked list */
void init_list(LinkedList* list) {
    list->head = NULL;
}

/* Function to add a node at the end of the linked list */
void add_node_end(LinkedList* list, pthread_t data) {
    Node* new_node = create_node(data);

    if (list->head == NULL) {
        /* If the list is empty, make the new node the head */
        list->head = new_node;
    } else {
        /* Traverse to the end of the list */
        Node* temp = list->head;
        while (temp->next != NULL) {
            /* if the node exists, return and don't add it*/
            if (pthread_equal(temp->data, data)) {
                return;
            }
            temp = temp->next;
            
        }
        /* Add the new node at the end if the node doesn't exist*/
        temp->next = new_node;
    }
}

/* Function to delete a node by its data (pthread_t) */
void delete_node(LinkedList* list, pthread_t data) {
    Node* temp = list->head;
    Node* prev = NULL;

    /* Traverse the list to find the node to delete */
    while (temp != NULL) {
        if (pthread_equal(temp->data, data)) {
            /* Node found, delete it */
            if (prev == NULL) {
                /* If the node to delete is the head */
                list->head = temp->next;
            } else {
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

/* Function to search for a node by its data (pthread_t) */
Node* search_node(LinkedList* list, pthread_t data) {
    Node* temp = list->head;

    /* Traverse the list to find the node */
    while (temp != NULL) {
        if (pthread_equal(temp->data, data)) {
            /* Node found */
            return temp;
        }
        temp = temp->next;
    }

    /* If the node was not found */
    return NULL;
}

pthread_t get_first_node(LinkedList* list){
    return list->head->data;
}

/* Function to print the linked list */
void print_list(LinkedList* list) {
    Node* temp = list->head;
    printf("Linked List: ");
    while (temp != NULL) {
        printf("%lu -> ", (unsigned long)temp->data);
        temp = temp->next;
    }
    printf("NULL\n");
}
