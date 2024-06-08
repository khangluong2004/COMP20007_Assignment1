/*
list.h

Visible structs and functions for linked lists.

Skeleton written by Grady Fitzpatrick for COMP20007 Assignment 1 2022
*/
/* The linked list. */
struct list;

/* Get a new empty list. */
struct list *newlist(void *item);

/* Add an item to the head of the list. Returns the new list. */
struct list *prependList(struct list *list, void *item);

/* Gets the first item from the list. */
void *peekHead(struct list *list);

/* Takes the first item from the list, updating the list pointer and returns
  the item stored. */
void *deleteHead(struct list **list);

/* Free all list items. */
void freeList(struct list *list);
