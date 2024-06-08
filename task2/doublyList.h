/*
    Written by Grady Fitzpatrick for 
    COMP20007 Assignment 1 2024 Semester 1
    
    Header for module which contains doubly-linked list 
        specification data structures and functions.
*/

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

struct linkedList;

// Expose the linked list struct for iteration
struct linkedList {
    struct linkedListNode *head;
    struct linkedListNode *tail;
};

struct linkedListNode {
    /* IMPLEMENT HERE */
    struct linkedListNode *prevNode;
    struct linkedListNode *nextNode;
    void *item;
};

/* Return a new empty list. */
struct linkedList *newDoublyList();

/* Check if the list is empty */
int isEmpty(struct linkedList *list);

/* Insert at the head of the list */
void insertHead(struct linkedList *list, void *item);

/* Insert at the tail of the list */
void insertTail(struct linkedList *list, void *item);

/* Remove from the head */
void* deleteDoublyHead(struct linkedList *list);

/* Free all items in the given list. */
void freeDoublyList(struct linkedList *list);

#endif

