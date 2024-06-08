/*
utils.c

Implementations for helper functions to do with reading and writing.

Skeleton written by Grady Fitzpatrick for COMP20007 Assignment 1 2024
*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "graph.h"
#include "utils.h"

/* Default cost for edges. */
#define DEFAULTCOST 1

struct graphProblem {
  int numLocations;
  int numConnections;
  int startingLocation;
  int finalLocation;
  struct graph *graph;  
};

struct graphProblem *readProblem(FILE *file, enum problemPart part){
  int i;
  int startRoom;
  int endRoom;
  int edgeWeight;
  /* Allocate space for problem specification */
  struct graphProblem *problem = (struct graphProblem *)
    malloc(sizeof(struct graphProblem));
  assert(problem);

  /* First line of input is the number of rooms. */
  assert(fscanf(file, "%d", &(problem->numLocations)) == 1);
  /* Next line comprises number of connections between rooms. */
  assert(fscanf(file, "%d", &(problem->numConnections)) == 1);
  /* Next line comprises the start room. */
  assert(fscanf(file, "%d", &(problem->startingLocation)) == 1);
  /* If PART_A, or PART_B or PART_D, a destination line will be present. */
  if(part == PART_A || part == PART_B || part == PART_D){
    assert(fscanf(file, "%d", &(problem->finalLocation)) == 1);
  }

  /* Build graph number of rooms. */
  problem->graph = newGraph(problem->numLocations);
  /* Add all edges to graph. */
  for(i = 0; i < problem->numConnections; i++){
    /* In PART_B, PART_C and PART_D, the cost will also be given. */
    if(part == PART_A){
      assert(fscanf(file, "%d %d", &startRoom, &endRoom) == 2);
      addEdge(problem->graph, startRoom, endRoom, DEFAULTCOST);
    } else {
      assert(fscanf(file, "%d %d %d", &startRoom, &endRoom, &edgeWeight) == 3);
      addEdge(problem->graph, startRoom, endRoom, edgeWeight);
    }
  }

  return problem;
}

struct solution *findSolution(struct graphProblem *problem,
  enum problemPart part){
  return graphSolve(problem->graph, part, problem->numLocations,
    problem->startingLocation, problem->finalLocation);
}

void freeProblem(struct graphProblem *problem){
  /* No need to free if no data allocated. */
  if(! problem){
    return;
  }
  freeGraph(problem->graph);

  free(problem);
}

void freeSolution(struct solution *solution){
  /* No need to free if no data allocated. */
  if(! solution){
    return;
  }
  free(solution);
}
