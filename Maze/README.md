# Maze Path Planner

This project utilized the C++ STL library to develop a program, based on the A* algorithm, to determine the shortest path between source and destination cells within an environment with obstacles.
One of the main objectives of the project was to practice the use of basic data structures and fundamental algorithms from the STL library. The following rules were followed during development:

- The most suitable container(s) among the sequential containers (vector, deque, or list) and/or simple adaptations of these containers (stack or queue) were used; containers based on heaps (priority_queue) or binary search trees (set, multiset, map, etc.) were not used, as they were data structures not studied in the introductory course.

- Advanced C++ features not covered in the course, such as lambda expressions, were avoided.

- Whenever possible, the generic algorithms from the STL (for_each, find, sort, etc.) were used and not replaced by a similar code snippet implemented by the programmer using control loops (for, while, etc.).

The A* algorithm found the shortest path in a graph where the transition between connected nodes had an associated cost. In this case, each cell of the maze was a node in the graph. The cells were connected to their 8 neighboring cells, and the cost of moving from one cell to another was the distance between the centers of the cells.

---
This README was automatically generated from the provided project specifications.