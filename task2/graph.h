/*
graph.h

Visible structs and functions for graph construction and manipulation.

Skeleton written by Grady Fitzpatrick for COMP20007 Assignment 1 2024
*/

/* Definition of a graph. */
struct graph;

enum problemPart;

struct solution;

/* A particular solution to a graph problem. */
#ifndef SOLUTION_STRUCT
#define SOLUTION_STRUCT
struct solution {
  /* 
    PART_A 
    The total number of steps taken to reach the destination.
  */
  int damageTaken;
  /* 
    PART_B 
    The total cost of all the materials required to disarm all
    traps to reach the destination safely.
  */
  int totalCost;
  /* 
    PART_C 
    The gold required to hire artisans to make all materials.
  */
  int artisanCost;
  /* 
    PART D
    The minimum percentage increase in damage required to reach
    the minion's location (truncated to the integer floor of the 
    whole percentage)
  */
  int totalPercentage;
};
#endif

/* Which part the program should find a solution for. */
#ifndef PART_ENUM
#define PART_ENUM
enum problemPart {
  PART_A=0,
  PART_B=1,
  PART_C=2,
  PART_D=3
};
#endif

/* Creates an undirected graph with the given numVertices and no edges and
returns a pointer to it. NumEdges is the number of expected edges. */
struct graph *newGraph(int numVertices);

/* Adds an edge to the given graph. */
void addEdge(struct graph *g, int start, int end, int cost);

/* Find the solution for the given problem if the optimal path is taken.
  In PART_A, PART_B and PART_D, the graph will be the graph formed by 
  locations in the forest, lair or delve. In PART_C, the graph represents the
  graph formed by materials and the abilities of each artisan.
    In PART_A, edges will be the possible locations the rogue could move to
    in the forest.
    In PART_B, edges will be the traps that guard each room in the lair.
    In PART_C, edges represent the capability to make one material from 
    another.
    In PART_D, edges represent the percentage increase in damage multiplier
    which will occur by taking that path from the source room into the 
    destination room.
  numLocations is the number of locations or materials in the graph.
  In all parts, the party or rogue start in the startingLocation and the 
    destination is the finalLocation. For PART_C, you may ignore the 
    finalLocation.
 */
struct solution *graphSolve(struct graph *g, enum problemPart part,
  int numLocations, int startingLocation, int finalLocation);

/* Returns a new graph which is a deep copy of the given graph (which must be 
  freed with freeGraph when no longer used). */
struct graph *duplicateGraph(struct graph *g);

/* Frees all memory used by graph. */
void freeGraph(struct graph *g);

/* Frees all data used by solution. */
void freeSolution(struct solution *solution);



