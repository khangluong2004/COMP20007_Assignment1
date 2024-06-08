/*
problem2b.c

Driver function for Problem 2 Part B.

Skeleton written by Grady Fitzpatrick for COMP20007 Assignment 1 2022
*/
#include <stdio.h>
#include "utils.h"
#include "graph.h"

int main(int argc, char **argv){
  /* Read the problem in from stdin. */
  struct graphProblem *problem = readProblem(stdin, PART_B);
  /* Find the solution to the problem. */
  struct solution *solution = findSolution(problem, PART_B);

  /* Report solution */
  printf("%d\n", solution->totalCost);

  freeProblem(problem);
  freeSolution(solution);

  return 0;
}

