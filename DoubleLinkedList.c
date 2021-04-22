#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <ctype.h>
struct Data {
    int data;
};

/* define a linked list node */
typedef struct node {
    struct Data val; 
    struct node *next;
    struct node *previous;
} node_t;

int remove_last(node_t **ptr2_head_node);
int remove_first(node_t **ptr2_head_node);
void deleteNode(node_t **ptr2_head_node, node_t **ptr2_delete_node);
void remove_by_value(node_t ** ptr2_head_node, int data) ;
void append(node_t **ptr2_head_node, int data);
void push(node_t **ptr2_head_node, int data);
void insert_after(node_t **ptr2_previous_node, int data);
void insert_before(node_t **ptr2_head_node, node_t **ptr2_next_node, int data);
int validChecker(node_t *head_node, int data);
int num_nodes(node_t *head_node);
int sum_nodes(node_t *head_node);
int max_nodes(node_t *head_node);
void print_list(node_t *head_node);
void delete_list(node_t *head_node);

/* The address of the head node will not change */
int remove_last(node_t **ptr2_head_node){
    int retval = 0; 
     /* if the linked list is empty, we print error message */
    if(*ptr2_head_node == NULL){
        printf("Error: Empty linked list\n");
        exit(EXIT_FAILURE);
    }
    /* 1. If there is only 1 node in the list, remove it */
    if((*ptr2_head_node)->next == NULL){
        retval = (*ptr2_head_node)->val.data;
        /* 2. Free the memory of the removed item */
        free(*ptr2_head_node);
        return retval;
    }
    node_t *traverse_node = *ptr2_head_node;
    /* 3. Check if the next node of traverse_node is point at NULL, if true then it's the last node */
    while(traverse_node->next->next != NULL){
        traverse_node = traverse_node->next;
    }
    /* 4. traverse_node is now the previous node of the last node */
    node_t *last_node = traverse_node->next;
    retval = last_node->val.data;
    /* 5. Free the memory of the removed item */
    free(last_node);
    /* 6. Make the previous node the last node of the list, which point at NULL */
    traverse_node->next = NULL;
    return retval;
}

/* The address of the head node will change */
int remove_first(node_t **ptr2_head_node){
    int retval = 0;
    /* 1. If the linked list is empty, we print error message */
    if(*ptr2_head_node == NULL){
        printf("Error: Empty linked list\n");
        exit(EXIT_FAILURE);
    }
    /* 2. If we remove the first node in the list, that makes the second node the first node */
    node_t *second_node = (*ptr2_head_node)->next;
    if(second_node == NULL){
       retval = (*ptr2_head_node)->val.data;
       free(*ptr2_head_node);
       *ptr2_head_node = NULL;
       return retval;
    }
    /* 3. The previous node before second node is NULL */
    second_node->previous = NULL;
    /* 4. Get the value and free the memory */
    retval = (*ptr2_head_node)->val.data;
    free(*ptr2_head_node);
    /* 5. Now assign the address of the head node to the address of second node, so we won't accidentally free the second node */
    *ptr2_head_node = second_node;
    return retval;
}

/* delete the node by the given address of the delete node */
void deleteNode(node_t **ptr2_head_node, node_t **ptr2_delete_node){
    /* 1. Deal with the problem linked list is empty and delete node is empty */
    if(*ptr2_head_node==NULL){
        printf("Error: Empty linked list\n");
        exit(EXIT_FAILURE);
    }
    if(*ptr2_delete_node==NULL){
        printf("Error: Node that is going to be deleted cannot be empty\n");
        exit(EXIT_FAILURE);
    }
    /* 2. If the head node is the node we want to delete, we call function remove_first */
    if(*ptr2_head_node == *ptr2_delete_node){
        remove_first(ptr2_head_node);
        return;
    }
    /* 3. If delete node is not the last node, we make the node after delete node point at the node before delete node */
    if((*ptr2_delete_node)->next != NULL){
        (*ptr2_delete_node)->next->previous = (*ptr2_delete_node)->previous;
    }
    /* 4. Make the node before delete node point at the node after delete node, since delete node is not the head node. */  
       (*ptr2_delete_node)->previous->next = (*ptr2_delete_node)->next;
    /* 5. If the delete node is the last node, just free it */
    free(*ptr2_delete_node);
}

/* Linked list allowed multiple, so we could recursively call this function to remove all the
 node that contains the value in the list. */
void remove_by_value(node_t **ptr2_head_node, int data) {
    /* if the linked list is empty, we print error message */
    if(*ptr2_head_node == NULL){
        printf("Error: Empty linked list\n");
        exit(EXIT_FAILURE);
    }
    /* if the item in the first node is same to value, we call function remove_first */
    if((*ptr2_head_node)->val.data == data){
         remove_first(ptr2_head_node);
         return;
    }
    /* target node is the node that contains the value we want to remove */
    node_t *traget_node = (*ptr2_head_node)->next;
    /* the loop terminate when target node is NULL */
        while (traget_node != NULL){
        /* We found our target node !!*/
             if(traget_node->val.data == data){
        /* Assume that target node is not the last node in the list, which means there are node before it, 
        we make the next node before target node point at the previous node of target node */
                 if(traget_node->next != NULL){
                     traget_node->next->previous = traget_node->previous;
                 }
                /* The previous node of target node is now point at the node after target node */
                 traget_node->previous->next = traget_node->next;
                 free(traget_node);
                 return;
             }
        /* update previous node to point at traverse_node and traverse_node point 
        at the node after it */
        traget_node = traget_node->next;
        }
    }

/* The address of the head node will not change, insert node at the end */
void append(node_t **ptr2_head_node, int data){
    node_t *last_node = *ptr2_head_node;
    /* 1. Allocate memory for new node */
    node_t *new_node = (node_t *) malloc(sizeof(node_t));
    if(new_node != NULL){
       new_node->val.data = data;
       /* Because new node is the last node, which means new node next point at NULL */
       new_node->next = NULL;
    }else{
        printf("Error: Memory allocation failure\n");
        exit(EXIT_FAILURE);
    }
    /* 2. If the Linked List is empty, then make the new node as head, which means 
    previous point at NULL and head node is NULL */
    if(*ptr2_head_node == NULL){
        *ptr2_head_node = new_node;
        new_node->previous = NULL;
        return;
    }
    /* 3. Iterate to the last node in the linked list */
    while(last_node->next != NULL){
        last_node = last_node->next;
    }
    /* 4. The last node points at the address of the new node */
    last_node->next = new_node;
    /* 5. Now new node should point at the last node */
    new_node->previous = last_node;
}

/* The address of head node will change, insert node before the head node */
void push(node_t **ptr2_head_node, int data){
    /* 1. Allocate memory for new node */
    node_t *new_node = (node_t *) malloc(sizeof(node_t));
     if(new_node != NULL){
        new_node->val.data = data;
        /* 2. Since the new node is the first node in the list, then the previous node before it is NULL */
        new_node->previous = NULL;
     }else{
        printf("Error: Memory allocation failure\n");
        exit(EXIT_FAILURE);
    }
    /* 3. The next node after new node should be head node or NULL */
    new_node->next = *ptr2_head_node;
    /* 4. If the linked list is not empty, which means head node is not NULL, then new node should be the previous node of head node */
    if(*ptr2_head_node != NULL){
       (*ptr2_head_node)->previous = new_node;
    }
     /* 5. Assign the address of the head node to the new node */
    *ptr2_head_node = new_node;
} 

/* The address here we are changing is the address of the node that we are going to insert after it */
void insert_after(node_t **ptr2_previous_node, int data){
    /* 1. Check if the given prev_node is NULL */
    if(*ptr2_previous_node == NULL){
        printf("Error: Previous node cannot be empty\n");
        exit(EXIT_FAILURE);
    }
    /* 2. Allocate memory of the new node */
    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    if(new_node != NULL){
        new_node->val.data = data;
    }else{
        printf("Error: Memory allocation failure\n");
        exit(EXIT_FAILURE);
    }
    /* 3. new node should point at the next address previous node used to point at, node or NULL*/
    new_node->next = (*ptr2_previous_node)->next;
    /* 4. previous node now point at the address of new node */
    (*ptr2_previous_node)->next = new_node;
    /* 5. Make the previous node the node before new node */
    new_node->previous = *ptr2_previous_node;
    /* 6. If new node is not the last node in the list, 
    the next node after the previous node should point at new node */
    if(new_node->next != NULL){
       new_node->next->previous = new_node;
    }  
}

/* The address here we are changing is the address of the node that we are going to insert before it */
void insert_before(node_t **ptr2_head_node, node_t **ptr2_next_node, int data){
    /* 1. Check if the given next_node is NULL, if there is a head node, then head node cannot be NULL, 
    if next node is the head node, then head node cannot be NULL*/
    if(*ptr2_next_node == NULL){
        printf("Error: Next node cannot be empty\n");
        exit(EXIT_FAILURE);
    }
    /* 2. Allocate memory of the new node */
    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    if(new_node != NULL){
        new_node->val.data = data;
    }else{
        printf("Error: Memory allocation failure\n");
        exit(EXIT_FAILURE);
    }
    /* 3. Make new node point at the previous node next node used to point at, node or NULL */
    new_node->previous = (*ptr2_next_node)->previous;
    /* 4. Make new node the previous node of next node */
    (*ptr2_next_node)->previous = new_node;
    /* 5. Make next node the node after new node */
    new_node->next = *ptr2_next_node;
    /* 6. If the previous node of new node is not NULL, meaning that next node is not head node,
     we make the previous node of next node now point at new node*/
    if(new_node->previous != NULL){
       new_node->previous->next = new_node;
    }else{
        /* 7. If the previous node of new node is NULL, we make the new node the head node of the list */
        *ptr2_head_node = new_node;
    }  
}

/* Check if value exists in the linked list */
int valueChecker(node_t *head_node, int data){
    node_t  *traversePtr = head_node;
    while (traversePtr) {
        if(traversePtr->val.data == data){
            return -1;
        }
        traversePtr = traversePtr->next;
    }
    return 0;
}

/* Check if value exists in the linked list */
int indexChecker(node_t *head_node, int index){
    int numNode = num_nodes(head_node);
    if(index > numNode || index < 0){
        return 0;
    }
    return 1;
}

/* Check how many nodes exist inside the linked list */
int num_nodes(node_t *head_node){
    node_t *traversePtr = head_node;
    int num = 0;
    while (traversePtr) {
        num++;
        traversePtr = traversePtr->next;
    }
    return num;
}

/* Get all the value from linked list */
int sum_nodes(node_t *head_node){
    node_t *traversePtr = head_node;
    int sum = 0;
    while (traversePtr) {
        sum += traversePtr->val.data;
        traversePtr = traversePtr->next;
    }
    return sum;
}

/* Get the max value from linked list */
int max_nodes(node_t *head_node){
    node_t *traversePtr = head_node;
    int max = 0, temp = 0;
    while (traversePtr) {
        temp = traversePtr->val.data;
        if(temp > max){
            max = temp;
        }
        traversePtr = traversePtr->next;
    }
    return max;
}

void print_list(node_t *head_node) {
    /* last node is used to print the list backward*/
    node_t *last_node = NULL;
    printf("Traversal in inward direction: ");
    while (head_node) {
        /* print the value stored in the first node */
        printf("%d ", head_node->val.data);
        /* structure pointer now point at the second node */
        last_node = head_node;
        head_node = head_node->next;
    }
    printf("\nTraversal in backward direction: ");
    while (last_node) {
        /* print the value stored in the first node */
        printf("%d ", last_node->val.data);
        /* structure pointer now point at the second node */
        last_node = last_node->previous;
    }
}

void delete_list(node_t *head_node) {
    node_t  *traversePtr = head_node, *next_node = head_node;
    /* free the memory of each node in the list */
    while (traversePtr) {
        next_node = traversePtr->next;
        free(traversePtr);
        traversePtr = next_node;
    }
}

int main() {
    /* need to consider memory allocation failure */
    /* if we are storing char[], then we need to consider whitespace, use getchar() */
    
    /* Declare the first node */
    node_t *head_node = NULL;
    node_t **ptr2_head_node = &head_node;
    /* Assign value to the first node using append (only way) */
    append(ptr2_head_node, 2); //first node is 4
    push(ptr2_head_node, 3); //first node is 3, second node is 2
    append(ptr2_head_node, 4); //first node is 3, second node is 2, third node is 4
    push(ptr2_head_node, 1); //first node is 1, second node is 3, third node is 2, forth node is 4
    append(ptr2_head_node, 5); //first node is 1, second node is 3, third node is 2, forth node is 4, fifth node is 5
    push(ptr2_head_node, 2); //first node is 2, second node is 1, third node is 3, forth node is 2, fifth node is 4, sixth node is 5
    append(ptr2_head_node, 2);//first node is 2, second node is 1, third node is 3, forth node is 2, fifth node is 4, sixth node is 5, seventh node is 2
    
    /* Insert node before the forth node, which means the node with value 6 now become the new forth node, (the insertion don't change the address of the origional forth node!!) */
    node_t *next_node = head_node->next->next->next;
    node_t **ptr2_next_node = &next_node;
    //first node is 2, second node is 1, third node is 3, forth node is 6, fifth node is 2, sixth node is 4, seventh node is 5, eighth node is 2
    insert_before(ptr2_head_node, ptr2_next_node, 6);

    /* Since the address of the next node is not changed, next node is still the node with value 2, which is the fifth node in the list, and now the previous node is the third node in the list */
     node_t *previous_node = next_node->previous->previous;
     node_t **ptr2_previous_node = &previous_node;
    //first node is 2, second node is 1, third node is 3, forth node is 8, fifth node is 6, sixth node is 2, seventh node is 4, eighth node is 5, nineth node is 2
     insert_after(ptr2_previous_node, 8);

    /*test to find the num, max, sum of nodes in the list */
    int num = num_nodes(head_node);
    int max = max_nodes(head_node);
    int sum = sum_nodes(head_node);
    printf("There are %d nodes inside the linked list before modify. The max value is %d and the sum value is %d in the list.\n", num, max, sum);
    print_list(head_node);

    /*test remove the node in the list with value 2: Success, but limited*/
    while(valueChecker(head_node, 2)){
         remove_by_value(ptr2_head_node, 2);
    }
    // first node is 1, second node is 3, third node is 8, forth node is 6, fifth node is 4, fifth node is 5 

    int num2 = num_nodes(head_node);
    printf("\n\nThere are %d nodes inside the liked list after remove the node with value 2.\n", num2);
    print_list(head_node);

    /*test remove the first node and last node in the list*/
    remove_first(ptr2_head_node);
    remove_last(ptr2_head_node);
    //first node is 3, second node is 8, third node is 6, forth node is 4

    int num3 = num_nodes(head_node);
    printf("\n\nThere are %d nodes inside the liked list after remove first and last node\n", num3);
    print_list(head_node);

    /*test delete the third node in the list */
    node_t *delete_node = head_node->next->next;
    node_t **ptr2_delete_node = &delete_node;
    deleteNode(ptr2_head_node, ptr2_delete_node);
    //first node is 3, second node is 8, third node is 4

    int num4 = num_nodes(head_node);
    printf("\n\nThere are %d nodes inside the liked list after remove the third node.\n", num4);
    print_list(head_node);

    /*test delete the head node using delete node */
    deleteNode(ptr2_head_node, ptr2_head_node);
    //first node is 8, second node is 4

    int num5 = num_nodes(head_node);
    printf("\n\nThere are %d nodes inside the liked list after remove the third node.\n", num5);
    print_list(head_node);

    delete_list(head_node);

    //strcpy(new_node->value, n);
}