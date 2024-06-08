/*
    Written by William Sutherland for 
    COMP20007 Assignment 1 2023 Semester 1
    Modified by Grady Fitzpatrick
    
    Implementation for module which contains priority 
      queue-related data structures and functions.
*/
#ifndef PQ_H
#define PQ_H

/* Data structure to represent the priority queue */
struct pq;

/* Creates a priority queue */
struct pq *createPQ();

/* Inserts an item into a priority queue in log(N) time where N 
   is the number of elements inside */
void insert(struct pq *q, void *item, int priority);

/* Retrieves the element with the highest priority. This is done
   in log(N) time */
void *pull(struct pq *q);

/* Returns if the priority has any elements in it */
int isEmpty(struct pq *q);

/* Frees the memory associated with the priority queue */
void freePQ(struct pq *q);
#endif
