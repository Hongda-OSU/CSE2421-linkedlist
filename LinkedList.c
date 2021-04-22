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
} node_t;

int remove_last(node_t **ptr2_head_node);
int remove_first(node_t **ptr2_head_node);
int remove_by_index(node_t **ptr2_head_node, int index);
void remove_by_value(node_t ** ptr2_head_node, int data) ;
void append(node_t **ptr2_head_node, int data);
void push(node_t **ptr2_head_node, int data);
void insert_after(node_t **ptr2_previous_node, int data);
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
    /* 1. If there is only 1 item in the list, remove it */
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
    /* if the linked list is empty, we print error message */
    if(*ptr2_head_node == NULL){
        printf("Error: Empty linked list\n");
        exit(EXIT_FAILURE);
    }
    /* if we remove the first node in the list, that makes the second node the first node */
    node_t *second_node = (*ptr2_head_node)->next;
    if(second_node == NULL){
       retval = (*ptr2_head_node)->val.data;
       free(*ptr2_head_node);
       *ptr2_head_node = NULL;
       return retval;
    }
    /* get the fist node value and free the memory */
    retval = (*ptr2_head_node)->val.data;
    free(*ptr2_head_node);
    /* the address of the first node now become the address of the second node */
    *ptr2_head_node = second_node;
    return retval;

}

/* Important ! Assume that index is valid */
int remove_by_index(node_t **ptr2_head_node, int index){
    int retval = -1;
    /* call remove_first if index is 0 */
    if(index == 0){
        return remove_first(ptr2_head_node);
    }
    /* 1. target node will be the node that we need to remove, traverse node will be the node before 
    target node */
    node_t *target_node = NULL,  *traverse_node = *ptr2_head_node;
    /* 2. Reach the previous node of the target node */
    for(int i = 0; i<index-1; i++){
        if(traverse_node->next == NULL){
            printf("Error: Impossible to reach the index\n");
            exit(EXIT_FAILURE);
        }
        traverse_node = traverse_node->next;
    }
    /* 3. Assign the target node */
    target_node = traverse_node->next;
    retval = target_node->val.data;
    /* 4. Make the traverse node point at the next node of target node */
    traverse_node->next = target_node->next;
    /* 5. Free the memory */
    free(target_node);
    return retval;

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
    }else{
    /* traverse_node starts at second node and previous is about to point at traverse_node */
    node_t *traverse_node = (*ptr2_head_node)->next;
    node_t *previous_node = (*ptr2_head_node);
    /* iterate until node after the last node, which is NULL */
        while (traverse_node){
        /* if traverse_node contains the value, we remove it and make previous node point 
        at the next node traverse_node is point at */
             if(traverse_node->val.data == data){
                 /* Assume that traverse_node is the last node, then traverse_node->next is NULL, and previous_node->next point at NULL, which is correct */
                 previous_node->next = traverse_node->next;
                 free(traverse_node);
                 return;
             }
        /* update previous node to point at traverse_node and traverse_node point 
        at the node after it */
        previous_node = traverse_node;
        traverse_node = traverse_node->next;
        }
    }
} 

/* The address of the head node will not change, insert node at the end */
void append(node_t **ptr2_head_node, int data){
    node_t *last_node = *ptr2_head_node;
    /* 1. Allocate memory for new node */
    node_t * new_node = (node_t *) malloc(sizeof(node_t));
    if(new_node != NULL){
       new_node->val.data = data;
       /* Because new node is the last node, which means new node next point at NULL */
       new_node->next = NULL;
    }else{
        printf("Error: Memory allocation failure\n");
        exit(EXIT_FAILURE);
    }
    /* 2. If the Linked List is empty, then make the new node as head */
    if(*ptr2_head_node == NULL){
        *ptr2_head_node = new_node;
        return;
    }
    /* 3. Iterate to the last node in the linked list */
    while(last_node->next != NULL){
        last_node = last_node->next;
    }
    /* 4. The last node points at the address of the new node */
    last_node->next = new_node;
}

/* The address of head node will change, insert node before the head node */
void push(node_t **ptr2_head_node, int data){
    /* 1. Allocate memory for new node */
    node_t *new_node = (node_t *) malloc(sizeof(node_t));
     if(new_node != NULL){
        new_node->val.data = data;
     }else{
        printf("Error: Memory allocation failure\n");
        exit(EXIT_FAILURE);
    }
    /* 2. Change the address of the first node in the linked list to the address of the new node */
    new_node->next = *ptr2_head_node;
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
    /*  3. Make next of new node as next of prev_node */
    new_node->next = (*ptr2_previous_node)->next;
     /* 4. new_node is the next node of previous node */
    (*ptr2_previous_node)->next = new_node;
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
    /* create a structure pointer that point at the beginning of the linked list */
    node_t *traversePtr = head_node;
    while (traversePtr) {
        /* print the value stored in the first node */
        printf("%d ", traversePtr->val.data);
        /* structure pointer now point at the second node */
        traversePtr = traversePtr->next;
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

    // node_t *head_node = (node_t *) malloc(sizeof(node_t));
    // head_node->val.data = 1;
    // head_node->next = (node_t *) malloc(sizeof(node_t));
    // head_node->next->val.data = 3;
    // head_node->next->next = (node_t *) malloc(sizeof(node_t));
    // head_node->next->next->val.data = 2;
    // head_node->next->next->next = (node_t *) malloc(sizeof(node_t));
    // head_node->next->next->next->val.data = 4;
    // head_node->next->next->next->next = NULL;
    // node_t **ptr2_head_node = &head_node;

    node_t **ptr2_head_node = &head_node;
   
   append(ptr2_head_node, 1); // first node is 1
   push(ptr2_head_node, 2); //first node is 2, second node is 1
   append(ptr2_head_node, 3); //first node is 2, second node is 1, third node is 3
   push(ptr2_head_node, 4); //first node is 4, second node is 2, third node is 1, forth node is 3

    /* add new node with value 5 before the third node */
    node_t **ptr2_previous_node = &((*ptr2_head_node)->next->next);
    //first node is 4, second node is 2, third node is 1, forth node is 5, fifth node is 3
    insert_after(ptr2_previous_node, 5);
    
    /* add new node with value 6 before the forth node */
    ptr2_previous_node = &((*ptr2_previous_node)->next);
    //first node is 4, second node is 2, third node is 1, forth node is 5, fifth node is 6, sixth node is 3
    insert_after(ptr2_previous_node, 6);

   /* add new node with value 4 before the second node */
    node_t **ptr2_previous_node2 = &((*ptr2_head_node)->next);
    //first node is 4, second node is 2, third node is 4, forth node is 1, fifth node is 5, sixth node is 6, the seventh node is 3
    insert_after(ptr2_previous_node2, 4);
    
    //first node is 4, second node is 2, third node is 4, forth node is 1, fifth node is 5, sixth node is 6, the seventh node is 3, the eigth node is 4
    append(ptr2_head_node, 4);

    /*test to find the num, max, sum of nodes in the list */
       int num = num_nodes(head_node);
       int max = max_nodes(head_node);
       int sum = sum_nodes(head_node);
       printf("There are %d nodes inside the lniked list before modify. The max value is %d and the sum value is %d in the list.\n", num, max, sum);
       printf("The list before modify is: ");
       print_list(head_node);

    /*test remove the last node from the list: Success */
        //remove_last(ptr2_head_node);

    /*test remove the first node from the list: Success */
        //remove_first(ptr2_head_node);

    /*test remove the third(2+1) node from the list by index: Success(2) Caution: If a node is freed, 
    the node is no longer available to point at other node  */
       //remove_by_index(ptr2_head_node, 4);

    /*test remove the node in the list with value 4: Success, but limited*/
       while(valueChecker(head_node, 4)){
             remove_by_value(ptr2_head_node, 4);
       }
    // first node is 2, second node is 1, third node is 5, forth node is 6, fifth node is 3

    int num2 = num_nodes(head_node);
    printf("\n\nThere are %d nodes inside the linked list after remove the node with value 4.", num2);
    printf("\nThe list after modify is: ");
    print_list(head_node);

    // remove the first node and last node in the list
    remove_first(ptr2_head_node);
    remove_last(ptr2_head_node);
    // first node is 1, second node is 5, third node is 6
    int num3 = num_nodes(head_node);
    printf("\n\nThere are %d nodes inside the linked list after remove the first and last node.", num3);
    printf("\nThe list after modify is: ");
    print_list(head_node);

    // remove the second node in the list
    remove_by_index(ptr2_head_node, 1);
    // first node is 1, second node is 6
    
    /*test insert a new node to the end of list whose value is 5: Success(2) 
    Caution: If you want to assign value to a node, first allocate memory for it, 
    and then make the previous node point at it*/
       //append(ptr2_head_node, 5);

     /*test insert a new node to the start of list whose value is 0: Success */
       //push(ptr2_head_node, 0);

    /*test insert a new node before the second node(1+1) whose value is 2: Success */
       //insert_after(ptr2_head_node, 2);
       
    int num4 = num_nodes(head_node);
    printf("\n\nThere are %d nodes inside the linked list after remove the second node.", num4);
    printf("\nThe list after modify is: ");
    print_list(head_node);

    delete_list(head_node);

    //strcpy(new_node->value, n);
}