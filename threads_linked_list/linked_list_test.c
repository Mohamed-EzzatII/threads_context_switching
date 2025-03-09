 #include"threads_linked_list.h"

// /* Main function to test the linked list */
// int main() {
//     pthread_t thread1, thread2, thread3;
//     thread1 = pthread_self(); /* Get the ID of the current thread */
//     thread2 = thread1 + 1;    /* Simulate another thread ID */
//     thread3 = thread2 + 1;    /* Simulate another thread ID */

//     /* Add nodes to the linked list */
//     add_node_end(thread1);
//     add_node_end(thread2);
//     add_node_end(thread3);
//     print_list();

//     /* Search for a node */
//     Node* found_node = search_node(thread2);
//     if (found_node != NULL) {
//         printf("Node found with data: %lu\n", (unsigned long)found_node->data);
//     }

//     /* Delete a node */
//     delete_node(thread2);
//     print_list();

//     /* Search for the deleted node */
//     found_node = search_node(thread2);
//     if (found_node == NULL) {
//         printf("Node not found.\n");
//     }

//     return 0;
// }/* Main function to test two linked lists */
int main() {
    pthread_t thread1, thread2, thread3, thread4;
    thread1 = pthread_self(); /* Get the ID of the current thread */
    thread2 = thread1 + 1;    /* Simulate another thread ID */
    thread3 = thread2 + 1;    /* Simulate another thread ID */
    thread4 = thread3 + 1;    /* Simulate another thread ID */

    /* Create two linked lists */
    LinkedList list1, list2;
    init_list(&list1);
    init_list(&list2);

    /* Add nodes to the first linked list */
    add_node_end(&list1, thread1);
    add_node_end(&list1, thread2);
    printf("List 1 after adding nodes:\n");
    print_list(&list1);

    /* Add nodes to the second linked list */
    add_node_end(&list2, thread3);
    add_node_end(&list2, thread4);
    printf("List 2 after adding nodes:\n");
    print_list(&list2);

    /* Search for a node in the first linked list */
    Node* found_node = search_node(&list1, thread2);
    if (found_node != NULL) {
        printf("Node found in List 1 with data: %lu\n", (unsigned long)found_node->data);
    }

    /* Delete a node from the first linked list */
    delete_node(&list1, thread2);
    printf("List 1 after deleting a node:\n");
    print_list(&list1);

    /* Search for the deleted node in the first linked list */
    found_node = search_node(&list1, thread2);
    if (found_node == NULL) {
        printf("Node not found in List 1.\n");
    }

    /* Print both linked lists */
    printf("Final state of List 1:\n");
    print_list(&list1);
    printf("Final state of List 2:\n");
    print_list(&list2);

    return 0;
}