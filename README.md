# Garbage Collection Project - Systems1

## Overview

This class project implements a toy managed memory allocator, focusing on efficient memory management through garbage collection techniques. The primary goal is to design a system that minimizes fragmentation, maximizes performance, and simplifies memory management for client applications. The main goals of this project were to learn how memory management works on a low level and to stregthen my proficiency in C. Written by myself for the University of Denver Systems 1 course.

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
4. **Programming Proficiency**: Strengthened my proficiency in C programming, particularly in the context of systems programming and low-level memory management.
5. **Project Structuring and Documentation**: Learned the importance of structuring a project effectively and documenting my code for clarity and maintainability.

## Versions

### Version 1: Enhanced Memory Management
- **Functionality**: Utilized different memory allocation strategies by implementing `BEST_FIT` alongside `FIRST_FIT`.
- **Memory Dump**: Wrote the `duMemoryDump` function for more detailed information about memory blocks.
- **Block Tracking**: Introduced a structure to track block usage and sizes, aiding in memory management and debugging.

### Version 2: Managed Pointer System
- **Indirection**: Added a managed pointer system using a Managed List to facilitate dynamic memory management and maintain valid client pointers during memory movement.
- **Memory Allocation and Freeing**: Introduced new functions for managed memory allocation and freeing, improving performance and streamlining memory operations.

### Version 3: Generational Garbage Collection
- **Young Generation**: Implemented a system to manage memory blocks in 'young' generation heaps, optimizing for performance.
- **Minor Collection**: Added the `minorCollection` function for defragmentation, relocating live objects to a new heap to reduce fragmentation.

### Version 4: In-Place Compaction and Old Generation Management
- **Old Generation Heap**: Introduced an 'old' generation heap for improved memory management of older objects.
- **In-Place Compaction**: Enhanced the `duFree` function to effectively manage memory from both young and old heaps.
- **Enhanced Minor Collection**: Updated the `minorCollection` function to better manage the survival count for blocks, refining memory management strategies.

## Usage

To use the memory allocator, include the header files and link against the provided implementation. The testing scripts demonstrate allocation and deallocation, including major and minor garbage collection processes. 

