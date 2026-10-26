# CS 300 Project One: Runtime and Memory Analysis

## Problem and assumptions

The ABCU advising program needs to load course data, prevent duplicate course numbers, validate prerequisites, find a requested course, and display courses in alphanumeric order. Project One compared a **vector**, **hash table**, and **binary search tree (BST)** for these tasks.

Let **n** represent the number of courses, **p** the number of prerequisite references, and **b** the number of allocated hash buckets. If each course has at most a fixed number **c** of prerequisites, then **p ≤ cn**. A vector append is amortized O(1); hash operations are expected O(1) when the load factor is controlled, but can be O(n) in the worst case; and the unbalanced BST may become skewed.

The load analysis includes creating course objects, detecting duplicates, inserting courses, and validating prerequisite references after loading. This allows a prerequisite to refer to a course later in the input file.

## Overall performance comparison

| Data structure | Load and validation: typical | Load and validation: worst | Find one course: typical | Find one course: worst | Print courses in alphanumeric order | Stored memory |
| --- | --- | --- | --- | --- | --- | --- |
| Vector | O(n²) | O(n²) | O(n) | O(n) | O(n log n) to sort | O(n + p) |
| Hash table | O(n + p), or O(n) if prerequisites are bounded | O(n²) | O(1) average | O(n) | O(n log n) to sort keys | O(n + p + b) |
| Unbalanced BST | O((n + p) log n) when reasonably balanced | O(n²) when skewed | O(log n) when reasonably balanced | O(n) | O(n) in-order traversal | O(n + p) |

For a fixed maximum number of prerequisites per course, the typical BST load simplifies to O(n log n). Both the vector and hash table need a separate sorting operation to print the courses in order, while BST in-order traversal prints them naturally in sorted order.

## Operations considered in loading and validation

| Operation | Number of calls | Vector cost per call | Hash table worst per call | BST worst per call |
| --- | --- | --- | --- | --- |
| Open/initialize and close/return | Constant | O(1) | O(1) | O(1) |
| Read a row | n | O(1) | O(1) | O(1) |
| Parse a row and create a course | n | O(1) under bounded prerequisites | O(1) under bounded prerequisites | O(1) under bounded prerequisites |
| Check for duplicate course number | n | O(n) | O(n) | O(n) |
| Insert the course | n | Amortized O(1) | O(n) | O(n) |
| Validate a prerequisite reference | p | O(n) | O(n) | O(n) |

Under worst-case lookup or collision conditions, the total load and validation cost can reach **O(n²)** when prerequisites per course are bounded. Under expected hash-table conditions it is O(n + p), while a reasonably balanced BST needs O((n + p) log n). If the number of prerequisites per course is not bounded, parsing and validation must account for p separately.

## Memory requirements

- **Vector:** O(n + p) to store course records and their prerequisites; preparing the sorted output can require O(n) additional temporary space.
- **Hash table:** O(n + p + b) to store course records, prerequisites, and hash buckets; sorting keys can require O(n) additional temporary space.
- **BST:** O(n + p) for course records, prerequisites, and tree nodes; recursive in-order traversal uses O(h) auxiliary stack space, where h is the tree height.

## Strengths, limitations, and recommendation

**Vector:** Straightforward, contiguous storage with little structural overhead. The main disadvantage is linear search for course numbers, which makes repeated duplicate and prerequisite checks more expensive. The vector must also be sorted to print the courses in order.

**Hash table:** Fast expected insertion and search, which makes it attractive for course lookups and validation. However, hash collisions can worsen performance, and the table does not inherently retain course numbers in sorted order, so sorting is necessary before output.

**Binary search tree:** In-order traversal naturally prints courses in alphanumeric order in O(n) time. Searches typically take O(log n) when the tree remains reasonably balanced. However, a plain BST is not self-balancing, so unfavorable insertion order can lead to a skewed tree and O(n) searches.

**Recommendation:** Use a **binary search tree** for the ABCU advising program because the application emphasizes printing an ordered course list. The BST supports this directly through in-order traversal while also offering efficient lookup when the tree is reasonably balanced. For a larger application, a self-balancing BST would be a stronger option because it prevents the worst-case degradation of a skewed tree.

*Adapted from the CS 300 Project One pseudocode and runtime analysis prepared for this course.*
