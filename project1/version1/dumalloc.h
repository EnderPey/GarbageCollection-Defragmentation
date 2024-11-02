#ifndef DUMALLOC_H
#define DUMALLOC_H

#define FIRST_FIT 0
#define BEST_FIT 1


// The interface for DU malloc and free
void duInitMalloc();
void* duMalloc(int size);
void duFree(void* ptr);
void duMemoryDump();
void printFreeList();
void printMemoryBlockInfo();

#endif