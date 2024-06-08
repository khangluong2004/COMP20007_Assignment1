/*
problem2a.c

Driver function for Problem 2 Part A.

Skeleton written by Grady Fitzpatrick for COMP20007 Assignment 1 2024
*/
#include <stdio.h>
#include "utils.h"
#include "graph.h"

int main(int argc, char **argv){
  /* Read the problem in from stdin. */
  struct graphProblem *problem = readProblem(stdin, PART_A);
  /* Find the solution to the problem. */
  struct solution *solution = findSolution(problem, PART_A);

  /* Report solution */
  printf("%d\n", solution->damageTaken);

  freeProblem(problem);
  freeSolution(solution);

  return 0;
}

