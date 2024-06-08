# Task 1 Assignment 1

## Acknowledgements

The questions and skeleton code is provided by COMP20007 UniMelb Sem 1 2024, which is indicated in each file.

The main algorithm is located in convexHull.c. The main data structure is a simple linkedList. Both are tasks implemented by me.

The description below is extracted from the course's problem description (with a bit of note).

## Convex Hull

A convex hull is a concept from geometry and computational geometry that refers to the smallest convex set that contains a given set of points in a Euclidean space. Imagine you have a set of nails sticking out from a board; the convex hull is the shape you would get by stretching a rubber band around all the nails and letting it snap tight around them. The boundary created by the rubber band defines the convex hull.

More formally, a convex set is a set of points in which, for any two points within the set, the line segment connecting them lies entirely within the set. The convex hull of a set of points is then the smallest convex set that contains all those points. It is often visualized in two dimensions (2D) but can be extended to higher dimensions as well.

## Tasks

### Implement Jarvis March:
- Initialization:
    - Start by selecting the leftmost point among the given points as the starting point of the convex hull. If there are multiple leftmost points, choose the one with the lowest y-coordinate.

    - Add this point to the convex hull.

- March:

    - Iterate through all the points to find the next point on the convex hull.

    - To find the next point, start from the current point and choose the point that has the largest counterclockwise angle with respect to the current point.

    - This is done by comparing the "slopes" of the line segments formed by the current point and each of the other points. Utilizing cross product, we can simply check if the product is positive/ negative, using only multiplication

    - The point with the largest counterclockwise angle is the next point on the convex hull.

    - Add this next point to the convex hull.

- Termination:

    - Repeat the main loop until the next point on the convex hull is the same as the starting point.

    - Once the next point becomes the starting point, the algorithm terminates.

- Output:

    - The output of the algorithm is the list of points that form the convex hull, ordered in counterclockwise order.

<b> Time complexity: </b> The time complexity of the Jarvis March algorithm is O(nh), where n is the number of input points and h is the number of points on the convex hull. In the worst case, where all points are on the convex hull, the time complexity is O(n^2).

### Implement Graham's scan
- Initialization:

    - Start by selecting the point with the lowest y-coordinate (or the leftmost point in case of ties) as the pivot point.

    - Sort the remaining points based on their polar angles with respect to the pivot point. If two points have the same polar angle, the one closer to the pivot point should come first.

- Scan:

    - Iterate through the sorted points and add them to the convex hull one by one.

    - For each point, check if it forms a counterclockwise turn with the two previously added points on the convex hull.

    - If it forms a counterclockwise turn, add the point to the convex hull. Otherwise, remove the last point from the convex hull until a counterclockwise turn is formed.

    - Repeat this process until all points are scanned.

- Termination:

    - Once all points are scanned, the convex hull is complete.

- Output:

    - The output of the algorithm is the list of points that form the convex hull, ordered in counterclockwise order.

<b> Time complexity: </b> The time complexity of Graham's scan algorithm is O(n log n), where n is the number of input points. This complexity arises from the sorting step required to order the points based on their polar angles. The scanning step, where each point is checked and added to the convex hull, takes linear time.

### Analyze the practical runtime of the 2 agorithms
Included in the written-tasks.pdf

## Running code
1. Run "make problem1a" (for Javis March) or "make problem1b" (for Graham Scan)
2. Run with: "problem1a < testcases" (similar for problem1b). Change the command based on OS.
3. The test cases and expected answers are included in the appropriate folder