/*
    Skeleton written by Grady Fitzpatrick for 
    COMP20007 Assignment 1 2024 Semester 1
    
    Header for module which contains convex hull 
        specification data structures and functions.

    Implemented by Luong An Khang
*/
#include "linkedList.h"
#include "problem.h"
#include "convexHull.h"
#include "stack.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#define LESS 1
#define LARGER -1
#define EQUAL 0

enum orientationResult {
    COLLINEAR = 0,
    CLOCKWISE = 1,
    COUNTERCLOCKWISE = 2
};

/* Finds the orientation between the three given points - calculates the angle between 
    the Middle-First vector and Middle-Final vector - if the Middle-First vector is zero-length,
    the gradient of the Middle-Final vector is instead used. */
enum orientationResult orientation(struct problem *p, int idxFirst, int idxMiddle, int idxFinal);

enum orientationResult orientation(struct problem *p, int idxFirst, int idxMiddle, int idxFinal) {
    assert(idxFirst >= 0 && idxFirst < p->numPoints);
    assert(idxMiddle >= 0 && idxMiddle < p->numPoints);
    assert(idxFinal >= 0 && idxFinal < p->numPoints);

    /* Use cross-product to calculate turn direction. */
    long double p0x = p->pointsX[idxFirst];
    long double p0y = p->pointsY[idxFirst];

    long double p1x = p->pointsX[idxMiddle];
    long double p1y = p->pointsY[idxMiddle];

    long double p2x = p->pointsX[idxFinal];
    long double p2y = p->pointsY[idxFinal];

    /* Cross product of vectors P1P0 & P1P2 */
    long double crossProduct = (p0x - p1x) * (p2y - p1y) - (p0y - p1y) * (p2x - p1x);

    if(crossProduct == 0) {
        if(idxFirst == idxMiddle) {
            /* Special case where we are only looking for positive slope of P1P2. */
            if(p2x == p1x) {
                /* Special case: dx = 0, vertical */
                if(p2y < p1y) {
                    /* Directly upwards */
                    return COUNTERCLOCKWISE;
                } else if(p2y == p1y) {
                    /* Same point. */
                    return COLLINEAR;
                } else {
                    return CLOCKWISE;
                }
            }
            long double m = (p2y - p1y) / (p2x - p1x);
            if(m >= 0) {
                return COUNTERCLOCKWISE;
            } else {
                return CLOCKWISE;
            }
            
        }
        return COLLINEAR;
    } else if(crossProduct > 0) {
        return CLOCKWISE;
    } else {
        return COUNTERCLOCKWISE;
    }
}

/* JARVIS'S MARCH */
/* Find the index of the leftmost point */
int leftmost(struct problem *p) {
    assert(p->numPoints > 0);

    // Linear search for the left most and lowest point
    int minPoint = 0;
    for (int i=1; i < p->numPoints; i++) {
        if (p->pointsX[i] < p->pointsX[minPoint]) {
            minPoint = i;
        } else if (p->pointsX[i] == p->pointsX[minPoint]) {
            if (p->pointsY[i] < p->pointsY[minPoint]) {
                minPoint = i;
            }
        }
    }

    return minPoint;
}

struct solution *jarvisMarch(struct problem *p) {
    /* Part A - perform Jarvis' March to construct a convex
    hull for the given problem. */

    /* IMPLEMENT HERE */
    /* The implementation uses the index of a point in the
    array of points stored in p to represent a point. Otherwise,
    the function follows strictly the given pseudocode */
    struct linkedList *hull = NULL;
    struct solution *s = (struct solution *) malloc(sizeof(struct solution));
    assert(s);
    s->operationCount = 0;

    // Ensure there are at least 3 points
    if (p->numPoints < 3) {
        return NULL;
    }

    // Initialize empty list
    hull = newList();

    // Find leftmost point index
    int idxLeftMost = leftmost(p);

    int idxCurrent = idxLeftMost;

    do {
        insertTail(hull, p->pointsX[idxCurrent], p->pointsY[idxCurrent]);

        // Initialize index for the nextPoint to be 0 
        int idxNext = 0;

        // Find the next point 'nextPoint' such that it forms a counterclockwise
        // turn with the current point and any other point in the set
        for (int i=1; i < p->numPoints; i++) {
            // Assume each point is unique for the index comparison

            // Check if equal to the current point. Split this 
            // into a separate if block since this doesn't involve
            // orientation comparison, thus should not affect the operationCount
            if (idxNext == idxCurrent) {
                idxNext = i;
                continue;
            }

            if (orientation(p, idxNext, idxCurrent, i) == COUNTERCLOCKWISE) {
                idxNext = i;
            }

            // Increment operationCount as we just used comparator
            s->operationCount++;
        }

        // Set 'nextPoint' as the current point for the next iteration
        idxCurrent = idxNext;
    
    // Repeat until we return to the starting point (leftmost)
    } while (idxCurrent != idxLeftMost);

    s->convexHull = hull;
    return s;
}

/* GRAHAM SCAN*/

/* Calculate the distance squared between 2 points */
double distanceSquared(int idxOne, int idxTwo, struct problem *p) {
    double diffX = p->pointsX[idxOne] - p->pointsX[idxTwo];
    double diffY = p->pointsY[idxOne] - p->pointsY[idxTwo];
    return diffX * diffX + diffY * diffY;
}

/* Comparator to compare between 2 points (represent by their indices in
the array of points stored in p) for sorting.
The point with lower polar angle to the lowest point, 
and lower distance will come first.
Return 1 if idxOne comes first, -1 if idxOne comes first, 0 if equal */
int sortComparator(int idxOne, int idxTwo, int idxLowest, struct problem *p, 
    int* operationCount) {
    // Check if any point is the lowest point
    if (idxOne == idxLowest) {
        return LESS;
    }

    if (idxTwo == idxLowest) {
        return LARGER;
    }

    // Check the orientation
    enum orientationResult compareOrient = orientation(p, idxOne, 
        idxLowest, idxTwo);
    // Increment operationCount
    *operationCount = *operationCount + 1;

    if (compareOrient == COUNTERCLOCKWISE) {
        // Point 1 is to the left of point 2, so point 2 comes first
        return LARGER;
    } else if (compareOrient == CLOCKWISE) {
        // Point 2 is to the left of point 1, so point 1 comes first
        return LESS;
    } else {
        // Equal orientation/ collinear so check the distance squared
        double distOne = distanceSquared(idxOne, idxLowest, p);
        double distTwo = distanceSquared(idxTwo, idxLowest, p);

        if (distOne < distTwo) {
            return LESS;
        } else {
            return LARGER;
        }
    }

    return EQUAL;
}

/* Helper function to merge two sorted part (from start to mid, and mid to end)  
of an array into 1 sorted array. Receive a temporary array for used
during the merging to avoid unnecessary allocate/ deallocate */
void mergeSortedArr(int *arr, int lenArr, int mid, 
    int *temp, struct problem *p, int idxLowest, int *operationCount) {

    int currFirst = 0;
    int currSecond = mid;
    int currTemp = 0;

    // Construct the merged array in the temp first
    for (currTemp=0; currTemp < lenArr; currTemp++) {
        // Stop when 1 of the half is finished
        if (currFirst >= mid) {
            break;
        }

        if (currSecond >= lenArr) {
            break;
        }

        // Otherwise put the smaller current element
        // of 1 of the 2 halves into temp
        if (sortComparator(arr[currFirst], arr[currSecond], idxLowest,
            p, operationCount) == LESS) {
            temp[currTemp] = arr[currFirst];
            currFirst++;
        } else {
            temp[currTemp] = arr[currSecond];
            currSecond++;
        }
    }

    // Place the remaining items from first or second half into temp
    if (currFirst >= mid) {
        // Second half still has remaining items, first half is finished
        for (int i=currSecond; i < lenArr; i++) {
            temp[currTemp] = arr[i];
            currTemp++;
        }
    } else {
        // First half still has remaining items, second half is finished
        for (int i=currFirst; i < mid; i++) {
            temp[currTemp] = arr[i];
            currTemp++;
        }
    }

    // Copy the merged result from temp to array
    for (int i=0; i < lenArr; i++) {
        arr[i] = temp[i];
    }
}

/* Merge sort the array, with customized comparator implemented above */
void mergeSort(int *arr, int n, int *temp, 
        struct problem *p, int idxLowest, int *operationCount) {
    if (n <= 1) {
        return;
    }

    int mid = n/2;

    mergeSort(arr, mid, temp, p, idxLowest, operationCount);
    mergeSort(arr + mid, n - mid, temp, p, idxLowest, operationCount);
    mergeSortedArr(arr, n, mid, temp, p, idxLowest, operationCount);
}

/* Wrapper of the merge sort. Used to allocated the temp array used
for merge sort, and free it at the end to avoid unnecessary malloc/free */
void sortPolarAngle(int *idxArray, int lenIdxArr, 
        struct problem *p, int idxLowest, int *operationCount) {
    // Allocate a temp array for use during merging
    int *temp = (int *) malloc(sizeof(int) * lenIdxArr);
    assert(temp);
    mergeSort(idxArray, lenIdxArr, temp, p, idxLowest, operationCount);
    free(temp);
}


/* Find the index of lowest (and leftmost) point */
int lowest(struct problem *p) {
    assert(p->numPoints > 0);
    int idxLowest = 0;
    for (int i=1; i < p->numPoints; i++) {
        if (p->pointsY[i] < p->pointsY[idxLowest]) {
            idxLowest = i;
        } else if (p->pointsY[i] == p->pointsY[idxLowest] 
                && p->pointsX[i] < p->pointsX[idxLowest]) {
            idxLowest = i;
        }
    }

    return idxLowest;
}

/* Helper function to get the secondTop of the stack,
and converting the item back to the index placed in.
Require stack to have at least 2 elements (which is respected in 
Graham's scan).
Implement this way to respect the stack abstraction method provided */
int getSecondTop(struct stack **idxStackPtr) {
    assert(idxStackPtr != NULL);
    void *top = pop(idxStackPtr);

    assert(idxStackPtr != NULL);
    void *secondTop = pop(idxStackPtr);

    push(idxStackPtr, secondTop);
    push(idxStackPtr, top);

    assert(secondTop != NULL);
    return *((int *) secondTop);
}

/* Helper function to get the top element of the stack,
and converting the item stored back to the index placed in */
int getTop(struct stack **idxStackPtr) {
    assert(idxStackPtr != NULL);
    void *top = pop(idxStackPtr);

    push(idxStackPtr, top);

    assert(top != NULL);
    return *((int *) top);
}

struct solution *grahamScan(struct problem *p) {
    /* Part B - perform Graham's Scan to construct a convex
    hull for the given problem. */
    /* IMPLEMENT HERE */
    /* Beside the sorting implementation and representing points 
    using index in the array of points stored in p,  
    the remaining algorithm strictly follows the given pseudocode*/
    struct linkedList *hull = NULL;
    struct solution *s = (struct solution *) malloc(sizeof(struct solution));
    assert(s);
    s->operationCount = 0;

    // Ensure there are at least 3 points
    if (p->numPoints < 3) {
        return NULL;
    }

    hull = newList();
    // Find lowest point index
    int idxLowest = lowest(p);

    // Create an array containing the index of all points,
    // and perform sort on this array
    int *idxArray = (int *) malloc(sizeof(int) * p->numPoints);
    assert(idxArray);
    for (int i=0; i < p->numPoints; i++) {
        idxArray[i] = i;
    }

    // Swap idxLowest to the front, since we start at this point
    // when scanning through all points
    idxArray[idxLowest] = 0;
    idxArray[0] = idxLowest;
    
    // Sort the remaining points
    sortPolarAngle(idxArray + 1, p->numPoints - 1, p, idxLowest, 
        &(s->operationCount));


    // Set up the stack, and push the first 3 points onto the stack
    struct stack *idxStack = createStack(&idxArray[0]);
    push(&idxStack, &idxArray[1]);
    push(&idxStack, &idxArray[2]);


    // Iterate through all the sorted points
    for (int i=3; i < p->numPoints; i++) {
        // While the current point and the two points below the top of the stack
        // make a non-left turn, pop the top of the stack
        while (orientation(p, getSecondTop(&idxStack), getTop(&idxStack), 
            idxArray[i]) != COUNTERCLOCKWISE) {
            pop(&idxStack);
        }

        // Push the current point onto stack
        push(&idxStack, &idxArray[i]);
    }

    // Move all the points in the stack to hull
    // Repeat until the return from pop is NULL,  
    // which implies the stack is empty, then break out of the loop
    int remainStack = 1;
    while (remainStack) {
        void *topStack = pop(&idxStack);
        if (topStack == NULL) {
            remainStack = 0;
            continue;
        }

        int topIdx =  *((int *) topStack);
        insertHead(hull, p->pointsX[topIdx], p->pointsY[topIdx]);
    }

    // Free the index array
    free(idxArray);

    // Free the stack
    freeStack(idxStack);

    s->convexHull = hull;
    return s;
}

void freeSolution(struct solution *s) {
    if(! s) {
        return;
    }
    if(s->convexHull) {
        freeList(s->convexHull);
    }
    free(s);
}

