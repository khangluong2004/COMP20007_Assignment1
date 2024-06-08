/*
problem2d.c

Driver function for Problem 2 Part D.

Skeleton written by Grady Fitzpatrick for COMP20007 Assignment 1 2024
*/
#include <stdio.h>
#include "utils.h"
#include "graph.h"

int main(int argc, char **argv){
  /* Read the problem in from stdin. */
  struct graphProblem *problem = readProblem(stdin, PART_D);
  /* Find the solution to the problem. */
  struct solution *solution = findSolution(problem, PART_D);

  /* Report solution */
  printf("%d\n", solution->totalPercentage);

  freeProblem(problem);
  freeSolution(solution);

  return 0;
}

