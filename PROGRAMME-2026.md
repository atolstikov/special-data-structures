# Программа SDS 2026 года

Выписка учебных разделов из [DOCX](<editions/2026_sds/program/Special_structures_of_data_маг_1_семестр.docx>), на языке источника. Это навигационная выписка, не новая утверждённая программа. В исходнике не заполнены подпись утверждения и регистрационный номер; факт утверждения не установлен.

В документе: первый семестр, 198 часов, 66 аудиторных (22 лекции, 22 лабораторные, 22 семинары), 132 часа самостоятельной работы, 6 зачётных единиц.

## Содержание

### Section 1. Range Query Data Structures

**Topic 1.1. Introduction. Complexity Analysis of Data Structure Operations**

Abstract Data Types (ADTs). The concept of a data structure interface. Defining usage goals and key performance metrics for data structures. Worst-case time complexity analysis. Amortized time complexity analysis. The method of potentials. Analysis of runtime on random inputs. Examples of data structures with balanced operation times. Examples of data structures balancing memory usage and runtime.

**Topic 1.2. Segment Tree**

The Segment Tree data structure. Implementation specifics in dynamic memory and on a fixed array. Decomposition of a query range into elementary segments. Point update operations. Query operations: point value, range sum, range minimum, count of distinct elements in a range. The cascading method. Operation complexity analysis.

**Topic 1.3. Segment Tree. Range Updates**

Specifics of applying range update operations: setting values on a range, adding values to a range. Query operations: point value, range sum, range minimum, count of distinct elements in a range. Operation complexity analysis. Application in solving geometric problems.

### Section 2. Data Decomposition

**Topic 2.1. Bitwise Representation of a Set**

Implementation of bitmask set representation and its operations. Efficiency analysis of operations. Solving systems of linear equations. Finding the rank of a matrix. Finding the transitive closure in a graph. Breadth-First Search (BFS) in a directed graph.

**Topic 2.2. Query Decomposition**

General principle of the query decomposition technique, “simple” operations, data structure rebuilding. Efficiency analysis of the method. Determining the optimal threshold value. Solving the problem of counting triangles in a graph. Solving the problem of finding the Lowest Common Ancestor (LCA) in a dynamically built tree.

**Topic 2.3. Efficient Computation Organization in Brute-Force Algorithms**

The “big step, little step” method. Solving the discrete logarithm problem with O(√n) complexity. Cycle detection. The “meet-in-the-middle” method. Solving the subset sum problem. Counting Hamiltonian cycles. Counting pairs of items with a fixed set of attributes.

### Section 3. Persistent Data Structures

**Topic 3.1. Classification. General Principles. General Methods for Building Persistent Data Structures**

Brief characteristics of persistent data structures. Classification of persistence: partial, full, confluent, functional. The full copying method. The path copying method. The “fat node” method.

**Topic 3.2. Persistent Stack, List, Queue**

Building a fully persistent stack based on a rooted tree. Converting a list to a partially persistent one in constant time. Implementing a queue interface using two stacks while preserving amortized complexity. Implementing a queue interface using six stacks with guaranteed worst-case complexity. Implementing a persistent queue using five stacks.

**Topic 3.3. Persistent Priority Queue, Segment Tree, Binary Search Tree**

Building a persistent segment tree using the path copying method. Building a persistent binary search tree using the path copying method. Leftist heaps. Binomial heaps. Building a persistent priority queue.

**Topic 3.4. Constructing Partially and Fully Persistent Data Structures**

Advantages and disadvantages of different versions of persistent data structures. A general method for building partially persistent data structures. Euler Tour of a tree. Maintaining order in a list. Obtaining fully persistent data structures.

### Section 4. Additional Topics

**Topic 4.1. Specialized Applied Data Structures**

Skip lists. LSM-tree. 2-3 tree. R-tree, R+-tree, R*-tree. Brodal–Okasaki heap. Dynamic connectivity.

## Темы лабораторных

1. Implementation of a stack/queue/deque supporting min-value queries.

2. Implementation of the Segment Tree ADT according to a given interface.

3. Implementation of the Segment Tree ADT with range updates.

4. Implementation of BFS in a directed graph using bitmask representations.

5. Implementation of an algorithm for counting triangles in an undirected graph.

6. Implementation of a solution for the knapsack problem using the “meet-in-the-middle” method.

7. Implementation of a persistent stack.

8. Implementation of a persistent segment tree.

9. Implementation of a persistent priority queue and a persistent binary search tree.

10. Implementation of a tree serialization algorithm (Euler Tour).

11. Implementation of a tree deserialization algorithm from an Euler Tour sequence.

## Темы семинаров

1. 2D Segment Tree (Segment Tree for two-dimensional queries).

2. Interval Tree.

3. Fast digital trie (X-fast trie).

4. Ultra-fast digital trie (Y-fast trie).

5. Link/Cut Tree.

6. Skip lists.

7. LSM-tree (Log-Structured Merge-tree).

8. 2-3 tree.

9. R-tree, R+-tree, R*-tree.

10. Brodal–Okasaki heap.

11. Dynamic connectivity.

## Темы тестов и коллоквиума

Test No. 1: Segment Tree.

Test No. 2: Segment Tree. Range Updates.

Test No. 3: Query Decomposition.

Test No. 4: Efficient Computation Organization in Brute-Force Algorithms.

Test No. 5: Persistent Priority Queue, Segment Tree, Binary Search Tree.

Test No. 6: Specialized Applied Data Structures.

Colloquium: Methods of Organizing Special Data Structures and Techniques of Their Application.

## Экзаменационные вопросы

1. Algorithm complexity analysis. Worst-case complexity.

2. Algorithm complexity analysis. Amortized analysis.

3. Algorithm complexity analysis. Potential method.

4. The concept of an abstract data type (ADT) interface. Examples of interfaces balancing time complexity of methods and memory usage.

5. Time complexity analysis of the counter increment operation (changing bits in a counter).

6. The concept of the Segment Tree data structure. Implementation specifics in dynamic and static memory.

7. Method and analysis of query decomposition into elementary segments in a Segment Tree.

8. The “lazy propagation” method for modifications in a Segment Tree.

9. The update operation and the range sum query operation in the problem of finding the sum on a segment.

10. The update operation and the range minimum query operation in the problem of finding the minimum on a segment.

11. Batch update operations (set, add) on a segment with a single query. Range sum query.

12. Finding the maximum subarray sum (the segment with the maximum sum) in a sequence.

13. The element update operation in the problem of finding the segment with the maximum sum.

14. The method of storing large amounts of information in each tree node. Counting the number of elements less than a given value in an array segment.

15. Bitmask representation of sets.

16. Bitmask representation of sets. Counting the number of Hamiltonian cycles.

17. Bitmask representation of sets. Counting the number of item pairs with a fixed set of attributes.

18. Bitmask representation of sets. Transitive closure in a directed graph.

19. Bitmask representation of sets. Breadth-first search in a directed graph.

20. Solving a system of linear equations using bitmask representation of sets.

21. Calculating the rank of a matrix using bitmask representation of sets.

22. The method of data and query decomposition. Counting triangles in a graph.

23. The method of data and query decomposition. Calculating the height of a vertex in a rebuildable tree of a special type.

24. The meet-in-the-middle algorithm. Solving the discrete logarithm problem.

25. The meet-in-the-middle algorithm. Cycle finding in a sequence.

26. Persistent data structures. Definitions. Classification. Examples.

27. Persistent stack.

28. Persistent list.

29. Persistent queue.

30. Persistent priority queue.

31. Persistent segment tree. The path copying method.

32. Persistent binary search tree. The path copying method.

33. A general method for building partially persistent data structures.

34. Euler tour technique for trees.

35. Maintaining order in a list.

36. Obtaining fully persistent data structures.
