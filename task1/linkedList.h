/*
    Written by Grady Fitzpatrick for 
    COMP20007 Assignment 1 2024 Semester 1
    
    Header for module which contains doubly-linked list 
        specification data structures and functions.
*/

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

struct linkedList;

/* Print out each value in the list from the first value to the final value.
    Each value is printed with the format "(x, y) " where x is the x value
    set when inserting the item and y is the y value set when inserting 
    the item. */
void traverseRingForward(struct linkedList *list);

/* Print out first the first item of the list, then print out each value in 
    the list from the final value to the second value.
    Each value is printed with the format "(x, y) " where x is the x value
    set when inserting the item and y is the y value set when inserting 
    the item. */
void traverseRingBackwards(struct linkedList *list);

/* Return a new empty list. */
struct linkedList *newList();

/* Insert the given x, y pair at the head of the list */
void insertHead(struct linkedList *list, long double x, long double y);

/* Insert the given x, y pair at the tail of the list */
void insertTail(struct linkedList *list, long double x, long double y);

/* Free all items in the given list. */
void freeList(struct linkedList *list);

#endif

