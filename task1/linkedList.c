/*
    Skeleton written by Grady Fitzpatrick for 
    COMP20007 Assignment 1 2024 Semester 1
    
    Implementation details for module which contains doubly-linked list 
        specification data structures and functions.
    
    Implemented by Luong An Khang
*/

#include "linkedList.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct linkedListNode;

struct linkedList {
    struct linkedListNode *head;
    struct linkedListNode *tail;
};

struct linkedListNode {
    /* IMPLEMENT HERE */
    struct linkedListNode *prevNode;
    struct linkedListNode *nextNode;

    long double x;
    long double y;
};

/* Print out each value in the list from the first value to the final value.
    Each value is printed with the format "(x, y) " where x is the x value
    set when inserting the item and y is the y value set when inserting 
    the item. */
void traverseRingForward(struct linkedList *list){
    /* IMPLEMENT HERE */
    struct linkedListNode *current = list->head;
    while (current != NULL){
        printf("(%Lf, %Lf) ", current->x, current->y);
        current = current->nextNode;
    }
}

/* Print out first the first item of the list, then print out each value in 
    the list from the final value to the second value.
    Each value is printed with the format "(x, y) " where x is the x value
    set when inserting the item and y is the y value set when inserting 
    the item. */
void traverseRingBackwards(struct linkedList *list){
    /* IMPLEMENT HERE */
    if (list->head != NULL){
        printf("(%Lf, %Lf) ", list->head->x, list->head->y);
    }
    struct linkedListNode *current = list->tail;
    while (current != list->head){
        printf("(%Lf, %Lf) ", current->x, current->y);
        current = current->prevNode;
    }
}

/* Return a new empty list. */
struct linkedList *newList(){
    /* IMPLEMENT HERE */
    struct linkedList *list = NULL;
    list = (struct linkedList *) malloc(sizeof(struct linkedList));
    list->head = NULL;
    list->tail = NULL;
    return list;
}

/* Insert the given x, y pair at the head of the list */
void insertHead(struct linkedList *list, long double x, long double y){
    /* IMPLEMENT HERE */
    struct linkedListNode *newNode = (struct linkedListNode*) malloc(sizeof(struct linkedListNode));
    newNode->x = x;
    newNode->y = y;
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

/* Insert the given x, y pair at the tail of the list */
void insertTail(struct linkedList *list, long double x, long double y){
    /* IMPLEMENT HERE */
    struct linkedListNode *newNode = (struct linkedListNode*) malloc(sizeof(struct linkedListNode));
    newNode->x = x;
    newNode->y = y;
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

/* Free all items in the given list. */
void freeList(struct linkedList *list){
    /* IMPLEMENT HERE */
    struct linkedListNode* current = list->head;
    while (current != NULL){
        struct linkedListNode* next = current->nextNode;
        free(current);
        current = next;
    }
    free(list);
}
