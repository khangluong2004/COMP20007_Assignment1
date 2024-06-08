/*
problem2c.c

Driver function for Problem 2 Part C.

Skeleton written by Grady Fitzpatrick for COMP20007 Assignment 1 2022
*/
#include <stdio.h>
#include "utils.h"
#include "graph.h"

int main(int argc, char **argv){
  /* Read the problem in from stdin. */
  struct graphProblem *problem = readProblem(stdin, PART_C);
  /* Find the solution to the problem. */
  struct solution *solution = findSolution(problem, PART_C);

  /* Report solution */
  printf("%d\n", solution->artisanCost);

  freeProblem(problem);
  freeSolution(solution);

  return 0;
}

