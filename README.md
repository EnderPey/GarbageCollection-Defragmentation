# Garbage Collection Project

## Overview

This project implements a managed memory allocator, focusing on efficient memory management through garbage collection techniques. The primary goal is to design a system that minimizes fragmentation, maximizes performance, and simplifies memory management for client applications.

### Key Features

- **Memory Allocation Strategies**: Implemented `FIRST_FIT` and `BEST_FIT` strategies to efficiently allocate memory blocks.
- **Managed Pointer System**: Developed a Managed List that allows for indirection, facilitating garbage collection without breaking client pointers when memory is moved.
- **Major and Minor Collection Functions**: Created `majorCollection` and `minorCollection` functions to perform memory compaction, consolidating used blocks and relocating them within the heap.
- **Memory Dump Enhancements**: Improved memory dump functionalities to inspect free lists and track memory access, ensuring effective debugging and validation.

## Learning Outcomes

Throughout the development of this project, I gained significant insights and skills, including:

1. **Understanding of Memory Management**: Deepened my understanding of dynamic memory allocation and the challenges associated with fragmentation and garbage collection.
2. **Algorithmic Design**: Improved my ability to design algorithms for memory allocation and compaction, considering performance trade-offs and efficiency.
3. **Debugging Skills**: Enhanced my debugging skills through the implementation of memory dump functionalities and systematic testing of allocation and deallocation operations.
4. **Programming Proficiency**: Strengthened my proficiency in C/C++ programming, particularly in the context of systems programming and low-level memory management.
5. **Project Structuring and Documentation**: Learned the importance of structuring a project effectively and documenting my code for clarity and maintainability.

## Usage

To use the memory allocator, include the header files and link against the provided implementation. The testing scripts demonstrate allocation and deallocation, including major and minor garbage collection processes. Ensure to run tests to validate functionality and performance metrics.

