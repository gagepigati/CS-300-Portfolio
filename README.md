# CS 300: Data Structures and Algorithms

## ABCU Course Advising Program

This repository contains selected portfolio artifacts from CS 300. Both projects focused on designing a course advising application for ABCU's Computer Science department. The goal was to read course information from a file, display the courses in alphanumeric order, and look up individual courses and their prerequisites.

### Portfolio artifacts

* [**Project One: Runtime and Memory Analysis**](Project_One_Runtime_and_Memory_Analysis.md) compares the performance and memory requirements of a vector, hash table, and binary search tree (BST), and explains the choice of a BST for the advising program.
* [**Project Two: Course Advising Program**](ABCU_Advising_Program.cpp) provides the C++ program that loads course data, prints an alphanumeric list of courses using a BST, and displays the prerequisites of a selected course.
* [**Sample course data**](ABCU_Advising_Program_Input.csv) provides the example input used for local testing.

## Reflection

### What was the problem you were solving in the projects for this course?

In both projects, I worked on a course advising program that would help academic advisors review the courses in ABCU's Computer Science program. The program needed to load course information from a file, display a list of all courses in alphanumeric order, and show the title and prerequisites for a selected course. Project One focused on planning the algorithms and comparing data structures, while Project Two focused on implementing the program in C++.

### How did you approach the problem? Why are data structures important to understand?

I started by breaking the program into smaller tasks, including reading course records, storing them, searching for individual courses, and printing a sorted list. In Project One, I compared vectors, hash tables, and binary search trees to understand how each one would affect runtime and memory usage. I selected a binary search tree because its in-order traversal produces the course list in alphanumeric order without needing a separate sorting step. The project helped me see that choosing a data structure affects how efficiently a program can search, organize, and display information.

### How did you overcome any roadblocks you encountered during the activities or projects?

One important challenge was handling prerequisites that might refer to courses listed later in the input file. Checking each prerequisite immediately could incorrectly reject valid data, so I used an approach that reads the records first and then checks whether each referenced course exists. I also planned for invalid records, duplicate course numbers, and file-loading errors. Breaking those cases into separate checks made it easier to reason about how the program should behave and keep invalid data from replacing a valid course list.

### How has this work expanded your approach to designing software and developing programs?

These projects taught me to think about the design of a program before focusing on the code. Instead of choosing the easiest structure to implement, I now consider the operations the program needs most often and compare their time and memory requirements. I also gained a better understanding of the difference between average-case and worst-case performance. For example, a regular binary search tree can search efficiently when it stays reasonably balanced, but its performance can get worse if the tree becomes heavily skewed. I will use that kind of analysis when making design choices in future projects.

### How has this work changed how you write maintainable, readable, and adaptable programs?

I have learned to separate responsibilities so that each part of a program has a clear purpose. Keeping file loading, validation, searching, and output in separate functions makes the code easier to understand and troubleshoot. Using a consistent course record and meaningful names also helps when revisiting a project later. I now see how important it is to handle errors without damaging existing data and to write code that could be extended in the future, such as supporting additional course information or replacing a regular BST with a self-balancing tree.

## Build and run

A C++17 compiler is required. From the directory containing the files, run:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic ABCU\_Advising\_Program.cpp -o advising
./advising
```

On Windows, run `advising.exe` after compiling. In the program, select option `1` and enter `ABCU\_Advising\_Program\_Input.csv` to load the data. Select `2` to print the sorted course list, `3` to look up a course and its prerequisites, or `9` to exit.

