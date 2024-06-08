/*
graph.c

Set of vertices and edges implementation.

Implementations for helper functions for graph construction and manipulation.

Skeleton written by Grady Fitzpatrick for COMP20007 Assignment 1 2024
Further implementation by Luong An Khang
*/
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <float.h>
#include <math.h>
#include "graph.h"
#include "utils.h"
#include "pq.h"
#include "doublyList.h"

#define INITIALEDGES 32
#define INIT_SUM 0.0 // The inital value for cumulative sum
#define INIT_PRODUCT 1.0 // The inital value for cumulative product
#define PERCENT_CONVERT 100 

struct edge;

/* Definition of a graph. */
struct graph {
	int numVertices;
	int numEdges;
	int allocedEdges;
	struct edge **edgeList;
};

/* Definition of an edge. */
struct edge {
	int start;
	int end;
	int cost;
};

struct graph *newGraph(int numVertices) {
	struct graph *g = (struct graph *) malloc(sizeof(struct graph));
	assert(g);
	/* Initialise edges. */
	g->numVertices = numVertices;
	g->numEdges = 0;
	g->allocedEdges = 0;
	g->edgeList = NULL;
	return g;
}

/* Adds an edge to the given graph. */
void addEdge(struct graph *g, int start, int end, int cost) {
	assert(g);
	struct edge *newEdge = NULL;
	/* Check we have enough space for the new edge. */
	if((g->numEdges + 1) > g->allocedEdges) {
		if(g->allocedEdges == 0) {
			g->allocedEdges = INITIALEDGES;
		} else {
			(g->allocedEdges) *= 2;
		}
		g->edgeList = (struct edge **) realloc(g->edgeList,
			sizeof(struct edge *) * g->allocedEdges);
		assert(g->edgeList);
	}

	/* Create the edge */
	newEdge = (struct edge *) malloc(sizeof(struct edge));
	assert(newEdge);
	newEdge->start = start;
	newEdge->end = end;
	newEdge->cost = cost;

	/* Add the edge to the list of edges. */
	g->edgeList[g->numEdges] = newEdge;
	(g->numEdges)++;
}

/* Returns a new graph which is a deep copy of the given graph (which must be 
	freed with freeGraph when no longer used). */
struct graph *duplicateGraph(struct graph *g) {
	struct graph *copyGraph = (struct graph *) malloc(sizeof(struct graph));
	assert(copyGraph);
	copyGraph->numVertices = g->numVertices;
	copyGraph->numEdges = g->numEdges;
	copyGraph->allocedEdges = g->allocedEdges;
	copyGraph->edgeList = (struct edge **) 
	malloc(sizeof(struct edge *) * g->allocedEdges);
	assert(copyGraph->edgeList || copyGraph->numEdges == 0);
	int i;
	/* Copy edge list. */
	for(i = 0; i < g->numEdges; i++) {
		struct edge *newEdge = (struct edge *) malloc(sizeof(struct edge));
		assert(newEdge);
		newEdge->start = (g->edgeList)[i]->start;
		newEdge->end = (g->edgeList)[i]->end;
		newEdge->cost = (g->edgeList)[i]->cost;
		(copyGraph->edgeList)[i] = newEdge;
	}
	return copyGraph;
}

/* Frees all memory used by graph. */
void freeGraph(struct graph *g) {
	int i;
	for(i = 0; i < g->numEdges; i++) {
		free((g->edgeList)[i]);
	}
	if(g->edgeList) {
		free(g->edgeList);
	}
	free(g);
}

/* Helper functions and structs to convert 
from the graph representation using list of edge and 
vertics to the representation using adjacency list */

/* struct for the node on the linked list in the
adjacency list, containing the connected node and the edge weight */
struct listItem {
	int node;
	int weight;
};

/* Helper function to allocate a listItem node on the heap 
and return the pointer */
struct listItem *generateListItem(int node, int weight) {
	struct listItem *newListItem = (struct listItem*) 
		malloc(sizeof(struct listItem));
	assert(newListItem);
	newListItem->node = node;
	newListItem->weight = weight;
	return newListItem;
}


/* Convert the given graph g into an adjacency list:
an array of linkedList, with each index position i-th
storing the list of neighbours (and weight of corresponding edge) of 
node i-th */
struct linkedList **createAdjacencyList(struct graph *g) {
	struct linkedList **adjacencyList = (struct linkedList **) 
		malloc(sizeof(struct linkedList) * g->numVertices);
	assert(adjacencyList);
	
	// Initialize linked list for all vertices
	for (int i=0; i < g->numVertices; i++) {
		adjacencyList[i] = newDoublyList();
	}

	// Add edge to it
	for (int i=0; i < g->numEdges; i++) {
		int start = g->edgeList[i]->start;
		int end = g->edgeList[i]->end;
		int cost = g->edgeList[i]->cost;

		insertHead(adjacencyList[start], generateListItem(end, cost));
		insertHead(adjacencyList[end], generateListItem(start, cost));
	}

	return adjacencyList;
}

/* Helper function to free the adjacencyList */
void freeAdjacencyList(struct linkedList **adjacencyList, int n) {
	for (int i=0; i < n; i++) {
		freeDoublyList(adjacencyList[i]);
	}
	free(adjacencyList);
}

/* 2A FUNCTION: 
Algorithm chosen: Breadth-First Search 
Justification: We are interested in the path from startingLocation to 
finalLocation that minimize the number of edges between the startingLocation 
and finalLocation (since each edge deals a constant amount of cost/ damage). 
Both Breadth-First search and Dijkstra will be appropriate. However, 
Breadth-First Search's time complexity (O(V + E))
is lower than Dijkstra (up to O(V * E) with the inefficienct priorityQueue 
given), so Breadth-First Search's chosen.
*/

/* Node for the breadthFirstSearch queue */
struct queueNode {
	int node;
	int damage;
};

/* Helper function to allocate a queueNode on the heap and return the pointer */
struct queueNode *generateQueueNode(int node, int damage) {
	struct queueNode *newNode = (struct queueNode *) 
		malloc(sizeof(struct queueNode));
	assert(newNode);
	newNode->node = node;
	newNode->damage = damage;
	return newNode;
}

/* Breadth-first search to find the minimum distance.
Return the minimum step taken to reach finalLocation from startingLocation */
int breadthFirstSearch(struct graph *g, int startingLocation, 
	int finalLocation) {
	// Convert to adjacency list representation
	struct linkedList **adjacencyList = createAdjacencyList(g);

	// Set up the breadthFirstSearch queue using the doubly linked list
	struct linkedList *queue = newDoublyList();
	insertTail(queue, generateQueueNode(startingLocation, 0));

	// Set up the visited array, to mark if the node was visited, 
	// to avoid visiting a node twice.
	int *visited = (int *) malloc(sizeof(int) * g->numVertices);
	assert(visited);
	for (int i=0; i < g->numVertices; i++) {
		visited[i] = 0;
	}

	// Flag to check if the finalLocation has been found, 
	// which we can stop the search if found
	int foundFinal = 0;

	// Record the total damages/ edges in the optimal path 
	// from startingLocation to finalLocation
	int finalDamage = -1;

	while (!isEmpty(queue) && !foundFinal) {
		// Pick the node from top of the queue
		struct queueNode *nodeItem = (struct queueNode *) 
			deleteDoublyHead(queue);
		assert(nodeItem);
		int topNode = nodeItem->node;
		int topDamage = nodeItem->damage;
		free(nodeItem);

		// Mark the node as visited
		visited[topNode] = 1;

		// Process the top node neighbour
		struct linkedList *neighboursList = adjacencyList[topNode];
		struct linkedListNode *curNode = neighboursList->head;

		while (curNode != NULL && !foundFinal) {
			int curNodeItem = ((struct listItem*) (curNode->item))->node;

			// Check if this node has been visited before
			if (visited[curNodeItem]) {
				curNode = curNode->nextNode;
				continue;
			}

			// Check if this node is the finalLocation
			if (curNodeItem == finalLocation) {
				foundFinal = 1;
				finalDamage = topDamage + 1;
				break;
			}

			// Otherwise, add the unvisited node to the queue
			insertTail(queue, generateQueueNode(curNodeItem, topDamage + 1));
			curNode = curNode->nextNode;
		}
	}

	// Free the queue
	freeDoublyList(queue);

	// Free the adjacency list
	freeAdjacencyList(adjacencyList, g->numVertices);

	// Free visited list
	free(visited);

	return finalDamage;
}

/* General Dijkstra's algorithm for both 2B and 2D.
Double type is used for node cost/ priority, as it can ensure the precision 
for int type in 2B and accomodate for 2D fractional value.

The main difference in 2B and 2D lies simply in the intial value
given for the startingLocation (0 for 2B, 1 for 2D) and how to 
"add" the edgeWeight of edge (C, D) to the cost of node C and 
get the cost of node D. Thus, these values are parameterized.
 */

 /* Helper functions to allocate a node for the priority queue,
 and return the pointer */
int *generatePriorityQueueItem(int node) {
	int *newItem = (int *) malloc(sizeof(int));
	assert(newItem);
	*newItem = node;
	return newItem;
}

/* Helper function to compare 2 integer items */
int compareInt(void *elem1, void *elem2) {
	int value1 = *((int *) elem1);
	int value2 = *((int *) elem2);
	return (value1 == value2);
}

/* General dijkstra function to find the path with minimum total weight 
from startingLocation to the finalLocation. Return the 
minimum cost from startingLocation to finalLocation */
double dijkstra(struct graph *g, int startingLocation, int finalLocation, 
	double addWeight(double, int), double startingCost) {

	// Convert to adjacency list representation
	struct linkedList **adjacencyList = createAdjacencyList(g);

	// Set up the priorityQueue
	struct pq *priorityQueue = newPQ();
	enqueue(priorityQueue, generatePriorityQueueItem(startingLocation),
		startingCost);

	// Keep check of the visited nodes' count, to stop searching 
	// if all nodes are picked out from the priorityQueue as minimum.
	int visitedCount = 1;

	// Create and initialize the cost array for each node
	double *cost = (double *) malloc(sizeof(double) * g->numVertices);
	assert(cost);
	for (int i=0; i < g->numVertices; i++) {
		cost[i] = -1.0;
	}
	cost[startingLocation] = startingCost;

	// Mark if the node is used as min node, since
	// multiple entries of the same node (different cost) 
	// might present in the priority queue
	int *visited = (int *) malloc(sizeof(int) * g->numVertices);
	assert(visited);
	for (int i=0; i < g->numVertices; i++) {
		visited[i] = 0;
	}

	while (visitedCount < g->numVertices) {
		// Pick the minimum node from the priorityQueue,
		// and free the node
		int *minItem = (int *) deletemin(priorityQueue);
		int minNode = *minItem;
		free(minItem);

		// Check if the minNode is the target
		// If yes, then end the search
		if (minNode == finalLocation) {
			break;
		}

		// Check if already visited
		if (visited[minNode]) {
			continue;
		}

		// Update the visited array and the visited count
		visited[minNode] = 1;
		visitedCount++;

		// Process the neighbours (connected node) to the minNode
		struct linkedList *neighboursList = adjacencyList[minNode];
		struct linkedListNode *curNode = neighboursList->head;

		while (curNode != NULL) {
			// Update the neighbour
			struct listItem *neighbour = (curNode->item);
			int neighbourNode = neighbour->node;

			// Skip this node if it's already visited
			if (visited[neighbourNode]) {
				curNode = curNode->nextNode;
				continue;
			}

			// Find the newCost for this neighbourNode
			double newCost = addWeight(cost[minNode], neighbour->weight);

			// Check if the node's previous cost is not initialized 
			// or larger than the newCost
			if (cost[neighbourNode] < 0.0) {
				// Add to queue if not initialized
				cost[neighbourNode] = newCost;
				enqueue(priorityQueue, generatePriorityQueueItem(neighbourNode),
					newCost);
			} else if (cost[neighbourNode] > newCost) {
				// Update the priority/ cost if already intialized
				cost[neighbourNode] = newCost;
				updatePriorities(priorityQueue, &neighbourNode, newCost,
					compareInt);
			}

			curNode = curNode->nextNode;
		}
	}

	double minimumCost = cost[finalLocation];

	// Free the priorityQueue
	freePQ(priorityQueue);

	// Free all the arrays used
	free(cost);
	free(visited);

	// Free the adjacencyList
	freeAdjacencyList(adjacencyList, g->numVertices);

	return minimumCost;
}

/* 2B FUNCTION: 
Algorithm: Dijkstra's algorithm
Justification: The problem to find the shortest path
between startingLocation and finalLocation in a weighted,
non-negative graph is the problem which Dijkstra is designed 
to solve efficiently. 
*/

/* Since we reuse the general template above for Dijkstra, 
only a Specific weight adding function is needed.

2B Specific Weight adding function
to find the cost of a node c, given a connected node d's cost
and the weight of the edge connecting c and d */
double addWeight2B(double nodeCost, int edgeWeight) {
	return nodeCost + edgeWeight;
}

/* 2C FUNCTION: 
Algorithm: Prim's algorithm
Justification: The problem is to find a "network" that 
contains a path from one node to any other node, with minimum
total weight of all edges. 

We can justify that the "network" required is a minimum spanning tree.

Assume the desired "network" is not a spanning tree:
Since each edge is positive, then if there is a cyclic graph G statisfies 
the constraints given, then the spanning tree of G would still
allow paths from one node to all other node, yet having a strict subset
of G's edges, thus having lower total edges' weights, causing a contradiction
with the assumption.

So, the desired "network" must be a minimum spanning tree, which Prim
is designed to solve (probably more efficient with a heap as priorityQueue).
*/

/*  Prim's implementation. Return the total weight 
of the minimum spanning tree */
int prim(struct graph *g, int startingLocation) {
	// Convert the graph to adjacencyList representation
	struct linkedList **adjacencyList = createAdjacencyList(g);

	// Create a priorityQueue
	struct pq *priorityQueue = newPQ();

	// Set up the cost array storing the cost of each nodes,
	// with cost being the smallest edge connecting the 
	// current spanning tree with the node
	int *cost = (int *) malloc(sizeof(int) * g->numVertices);
	assert(cost);
	for (int i=0; i < g->numVertices; i++) {
		cost[i] = -1;
	}

	// Use the visited array to check if the node is included
	// in the spanning tree already
	int *visited = (int *) malloc(sizeof(int) * g->numVertices);
	assert(visited);
	for (int i=0; i < g->numVertices; i++) {
		visited[i] = 0;
	}
	int visitedCount = 0;

	// Total weight of the minimum spanning tree
	int minimumCost = 0; 
	
	// Initialise values of the startingLocation
	cost[startingLocation] = 0;
	enqueue(priorityQueue, generatePriorityQueueItem(startingLocation), 0);

	while (visitedCount < g->numVertices) {
		// Get the minimum node from the priorityQueue
		// to add to the tree
		int *minItem = (int *) deletemin(priorityQueue);
		int minNode = *minItem;
		free(minItem);

		// Check if vivited before
		if (visited[minNode] == 1) {
			continue;
		}

		// Update the visited flag, count, and total weight of the tree
		visited[minNode] = 1;
		minimumCost += cost[minNode];
		visitedCount++;

		// Process the neighbours of minNode
		struct linkedList *neighboursList = adjacencyList[minNode];
		struct linkedListNode *curNode = neighboursList->head;
		while (curNode != NULL) {
			// Check each neighbour
			struct listItem *neighbour = curNode->item;
			int neighbourNode = neighbour->node;
			int neighbourCost = neighbour->weight;

			// Skip this node if it's already visited
			if (visited[neighbourNode]) {
				curNode = curNode->nextNode;
				continue;
			}

			// Update the cost if the cost previously not initialized
			// or previous cost is higher than the new cost
			if (cost[neighbourNode] < 0.0 ) {
				// Add node to queue if not intialized
				cost[neighbourNode] = neighbourCost;
				enqueue(priorityQueue, generatePriorityQueueItem(neighbourNode),
					neighbourCost);
			} else if (cost[neighbourNode] > neighbourCost) {
				// Update the new lower priority/cost if already initialized
				cost[neighbourNode] = neighbourCost;
				updatePriorities(priorityQueue, &neighbourNode, neighbourCost,
					compareInt);
			}
			curNode = curNode->nextNode;
		}
	}

	// Free the arrays
	free(visited);
	free(cost);

	// Free the priority queue
	freePQ(priorityQueue);

	// Free the adjacencyList
	freeAdjacencyList(adjacencyList, g->numVertices);

	return minimumCost;
}


/* 2D FUNCTIONS:
Algorithm: Dijkstra (modified), changing
the adding weight method, instead of just adding:

Given node C being the minimum node from the priorityQueue,
and node D being its neighbour:
Cost of node D = Cost of node C * (1 + weight of CD in decimal)
(basically just change the weight CD from percentage to decimal
(original weight of CD/100), and multiply it to the cost of node C)

Justification: 
The goal is to find the minimum path, such that for
the weight w1, w2, w3, ..., w_n on the path:
W_total = (1 + w1) * (1 + w2) * (1 + w3) * ... * (1 + w_n) is minimized.

Note that, the goal is equivalent to minimize
log(W_total), since log is an increasing function on R.

Thus, the objective function can be:
log(W_total) = log(1 + w1) + log(1 + w2) + ... + log(1 + w_n)
Which means that if we convert the weight w of all edge on the graph
to log(1 + w), the problem converts to the original dijkstra's problem.

With this log-converted graph, the adding weight operation would 
be to add log(1 + w). On the original graph, this is equivalent to 
multiply with (1 + w). Since the correctness of doing dijkstra
traditionally on the log-converted graph is ensured, the correctness
of doing djikstra with the original graph but with the modified 
"adding weight" operation modiied should be followed, thanks to log
being an increasing function, thus guarateeing the same result
from all comparisons.
*/

/* 2D Specific adding weight function:
As specified above, the modified Dijkstra requires a different
"adding weight" operation */
double addWeight2D(double nodeCost, int edgeWeight) {
	return nodeCost * (1.0 + edgeWeight * 1.0/100);
}



struct solution *graphSolve(struct graph *g, enum problemPart part,
	int numLocations, int startingLocation, int finalLocation) {
	struct solution *solution = (struct solution *)
		malloc(sizeof(struct solution));
	assert(solution);
	if(part == PART_A) {
		/* IMPLEMENT 2A SOLUTION HERE */
		solution->damageTaken = breadthFirstSearch(g, startingLocation, 
			finalLocation);
	} else if(part == PART_B) {
		/* IMPLEMENT 2B SOLUTION HERE */
		solution->totalCost = dijkstra(g, startingLocation, finalLocation, 
			addWeight2B, INIT_SUM);
	} else if(part == PART_C) {
		/* IMPLEMENT 2C SOLUTION HERE */
		solution->artisanCost = prim(g, startingLocation);
	} else {
		/* IMPLEMENT 2D SOLUTION HERE */
		/* The cost found would be the cumulated multiplier, in decimal format 
		of 1.(...). The percentage increase requires a small calcualtion */
		double minimumCost = dijkstra(g, startingLocation, finalLocation, 
			addWeight2D, INIT_PRODUCT);
		int roundPercentage = ((int) (minimumCost * PERCENT_CONVERT))
			 - PERCENT_CONVERT;

		solution->totalPercentage = roundPercentage;
	}
	return solution;
}

