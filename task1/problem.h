/*
    Written by William Sutherland for 
    COMP20007 Assignment 1 2023 Semester 1
    Modified by Grady Fitzpatrick
    
    Header for module which contains problem 
        specification data structures and functions.
*/
#ifndef PROBLEM_H
#define PROBLEM_H

/* Included for FILE type. */
#include <stdio.h>

/* Which part the program should find a solution for. */
#ifndef PART_ENUM
#define PART_ENUM
enum problemPart {
  PART_A=0,
  PART_B=1
};
#endif

/* The struct for both problems */
struct problem {
    int numPoints;
    long double *pointsX;
    long double *pointsY;
    enum problemPart part;
};

/* Reads problem data from file */
struct problem *readProblem(FILE *f, enum problemPart part);

/* Frees the memory from the problem setup data. */
void freeProblem(struct problem *p);

#endif
