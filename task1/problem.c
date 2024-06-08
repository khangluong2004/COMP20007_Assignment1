/*
    Written by William Sutherland for 
    COMP20007 Assignment 1 2023 Semester 1
    Heavily modified by Grady Fitzpatrick for
    COMP20007 Assignment 1 2024 Semester 1
    
    Implementation for module which contains problem 
        specification data structures and functions.
*/

#include "problem.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct problem *readProblem(FILE *f, enum problemPart part) {
    struct problem *problem = (struct problem *) malloc(sizeof(struct problem));
    assert(problem);

    /* First line of input represents the number of points we are testing */
    assert(fscanf(f, "%d", &(problem->numPoints)) == 1);
    
    /* Allocate space for all points. */
    problem->pointsX = (long double *) malloc(problem->numPoints * sizeof(long double));
    assert(problem->pointsX || problem->numPoints == 0);
    problem->pointsY = (long double *) malloc(problem->numPoints * sizeof(long double));
    assert(problem->pointsY || problem->numPoints == 0);

    // Scans all the points
    for (int i = 0; i < problem->numPoints; i++) {
        assert(fscanf(f, "%Lf %Lf", &(problem->pointsX[i]), &(problem->pointsY[i])) == 2);
    }

    problem->part = part;

    return problem;
}

void freeProblem(struct problem *p) {
    if(!p){
        return;
    }
    if(p->numPoints != 0){
        free(p->pointsX);
        free(p->pointsY);
    }
    free(p);
}

