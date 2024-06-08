/*
    Written by William Sutherland for 
    COMP20007 Assignment 1 2023 Semester 1
    Modified by Grady Fitzpatrick
    
    Implementation for module which contains priority 
      queue-related data structures and functions.
*/
/*
Priority queue data structure. 
Implemented with a binary heap meaning pull and insert operations are both O(log(N))
*/

#include "pq.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define INITIAL_CAPACITY 64
#define CAPACITY_MULTIPLIER 2

/* A method to swap two nodes in the priority queue */
void swap(struct pq *p, int i, int j);

struct pq {
    int totalItems;
    int size;
    struct pqNode *nodes;
};

/* A data structure to store the node of a pq */
struct pqNode {
    void *item;
    int priority;
};

struct pq *createPQ() {
    struct pq *q = (struct pq *) malloc(sizeof(struct pq));
    assert(q);
    q->nodes = (struct pqNode *) malloc(INITIAL_CAPACITY * sizeof(struct pqNode));
    assert(q->nodes);
    q->size = INITIAL_CAPACITY;
    q->totalItems = 0;
    return q;
}

void insert(struct pq *q, void *item, int priority) {
    struct pqNode n;
    n.item = item;
    n.priority = priority;

    // Reallocates memory if the priority queue gets big
    (q->totalItems)++;
    if (q->totalItems > q->size) {
        q->size *= 2;
        q->nodes = (struct pqNode *) realloc(q->nodes, q->size * sizeof(struct pqNode));
        assert(q->nodes);
    }

    int curr = q->totalItems - 1;
    q->nodes[curr] = n;

    // The node starts at the back of the array and is 
    // moved up the heap until it is in the right place
    while (curr > 0) {
        int parent = (curr - 1) / 2;
        if (q->nodes[curr].priority > q->nodes[parent].priority) {
            swap(q, curr, parent);
            curr = parent;
        } else {
            break;
        }
    }
}

void *pull(struct pq *q) {
    void *itemPull = q->nodes->item;
    (q->totalItems)--;
    swap(q, 0, q->totalItems);

    // We moved the back node to the front and then we push it down the stack
    // until the stack condition is satisfied
    int curr = 0;
    while (curr < q->totalItems) {
        int highestPriority = curr;
        if ((2 * curr) + 1 < q->totalItems && q->nodes[2 * curr + 1].priority > q->nodes[highestPriority].priority){
            highestPriority = 2*curr+1;
        }
        if ((2 * curr) + 2 < q->totalItems && q->nodes[2 * curr + 2].priority > q->nodes[highestPriority].priority){
            highestPriority = 2*curr+2;
        }

        if (highestPriority == curr){
            break;
        }

        swap(q, curr, highestPriority);
        curr = highestPriority;
    }

    return itemPull;
}

int isEmpty(struct pq *q) {
    return q->totalItems == 0;
}

void freePQ(struct pq *q) {
    free(q->nodes);
    free(q);
}

void swap(struct pq *q, int i, int j) {
    struct pqNode temp = q->nodes[i];
    q->nodes[i] = q->nodes[j];
    q->nodes[j] = temp;
}
