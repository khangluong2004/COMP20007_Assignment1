/*
pq.h

Visible structs and functions for priority queues.

Skeleton written by Grady Fitzpatrick for COMP20007 Assignment 1 2022
*/
/* The priority queue. */
struct pq;

/* Get a new empty priority queue. */
struct pq *newPQ();

/* Add an item to the priority queue - cast pointer to (void *). */
void enqueue(struct pq *pq, void *item, double priority);

/* Take the smallest item from the priority queue - cast pointer back to
  original type. */
void *deletemin(struct pq *pq);

/* Scan through the queue linearly and update the first matching element.
Used only when the item is guaranteed to be unique */
void updatePriorities(struct pq *pq, void* updateElem, 
  double newPriority, int compare(void*, void*));

/* Returns 1 if empty, 0 otherwise. */
int empty(struct pq *pq);

/* Remove all items from priority queue (free the items) and free the queue. */
void freePQ(struct pq *pq);