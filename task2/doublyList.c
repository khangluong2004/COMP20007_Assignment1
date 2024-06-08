/*
    Adapted from skeleton written by Grady Fitzpatrick for 
    COMP20007 Assignment 1 2024 Semester 1
    
    Implementation details for module which contains doubly-linked list 
        specification data structures and functions.
    
    Implemented by <YOU>
*/

#include "doublyList.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct linkedListNode;

/* Return a new empty list. */
struct linkedList *newDoublyList(){
    /* IMPLEMENT HERE */
    struct linkedList *list = NULL;
    list = (struct linkedList *) malloc(sizeof(struct linkedList));
    list->head = NULL;
    list->tail = NULL;
    return list;
}

/* Check if list is empty */
int isEmpty(struct linkedList *list){
    return list->head == NULL; 
}

/* Insert at the head of the list */
void insertHead(struct linkedList *list, void *item){
    /* IMPLEMENT HERE */
    struct linkedListNode *newNode = (struct linkedListNode*) malloc(sizeof(struct linkedListNode));
    newNode->item = item;
    newNode->prevNode = NULL;

    newNode->nextNode = list->head;
    // Check if the list is not empty to change head's pointer
    if (list->head != NULL){
        list->head->prevNode = newNode;
    }
    list->head = newNode;

    // Handle empty list
    if (list->tail == NULL){
        list->tail = newNode;
    }
}

/* Insert at the tail of the list */
void insertTail(struct linkedList *list, void *item){
    /* IMPLEMENT HERE */
    struct linkedListNode *newNode = (struct linkedListNode*) malloc(sizeof(struct linkedListNode));
    newNode->item = item;
    newNode->nextNode = NULL;

    newNode->prevNode = list->tail;
    // Check if the list is not empty to change tail's pointer
    if (list->tail != NULL){
        list->tail->nextNode = newNode;
    }
    list->tail = newNode;

    // Handle empty list
    if (list->head == NULL){
        list->head = newNode;
    }
}

/* Remove from the head */
void* deleteDoublyHead(struct linkedList *list){
    assert(list != NULL);
    if (list->head == NULL){
        return NULL;
    }

    struct linkedListNode* oldHead = list->head;
    void* oldItem = oldHead->item;
    list->head = list->head->nextNode;
    free(oldHead);

    // Check if list is empty
    if (list->head == NULL){
        list->tail = NULL;
    } else {
        list->head->prevNode = NULL;
    }

    return oldItem;
}


/* Free all items in the given list. */
void freeDoublyList(struct linkedList *list){
    /* IMPLEMENT HERE */
    struct linkedListNode* current = list->head;
    while (current != NULL){
        struct linkedListNode* next = current->nextNode;
        free(current->item);
        free(current);
        current = next;
    }
    free(list);
}
