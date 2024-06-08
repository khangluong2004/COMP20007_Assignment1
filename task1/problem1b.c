/*
    Written by Grady Fitzpatrick for 
    COMP20007 Assignment 1 2024 Semester 1
    
    Make using
        make problem1b
    
    Run using
        ./problem1b < test_case
    
    where test_case is a test case in the expected
        format (e.g. test_cases/1b-1.txt), for example:
    
        ./problem1b < test_cases/1b-1.txt
*/
#include "convexHull.h"
#include "problem.h"
#include "linkedList.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    struct problem *problem = readProblem(stdin, PART_B);

    struct solution *solution = grahamScan(problem);

    /* Print points in clockwise order. */
    traverseRingBackwards(solution->convexHull);
    printf("\n");

    /* Print points in counterclockwise order. */
    traverseRingForward(solution->convexHull);

    printf("\n");

    // printf("Total operations: %d \n", solution->operationCount);

    freeSolution(solution);
    freeProblem(problem);

    return EXIT_SUCCESS;
}
