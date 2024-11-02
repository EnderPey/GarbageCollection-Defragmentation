#include <stdio.h>
#include "dumalloc.h"

#define HEAP_SIZE (128*8)


int allocationStrategy;
unsigned char heap[HEAP_SIZE];





typedef struct memoryBlockHeader {
    int free; // 0 - used, 1 = free
    int size; // size of the reserved block
    int managedIndex; // the unchanging index in the managed array
    struct memoryBlockHeader* next;
} memoryBlockHeader;

memoryBlockHeader* freeListHead;
memoryBlockHeader* managed_list[HEAP_SIZE / 8];
int managed_size;

void duInitMalloc(int strategy){
    allocationStrategy = strategy;

    for(int i =0; i<HEAP_SIZE; i++){
        heap[i] = 0;
    }
    memoryBlockHeader* currentBlock = (memoryBlockHeader*)heap;

    currentBlock->size = HEAP_SIZE - sizeof(memoryBlockHeader);
    currentBlock->next = NULL;
    currentBlock->free = 1;
    freeListHead = currentBlock;
}

void printMemoryBlockInfo(){
    memoryBlockHeader* currentBlock = (memoryBlockHeader*)heap;
    char output[HEAP_SIZE / 8];
    int index = 0;
    char initial_free = 'a';
    char initial_used = 'A';


    while((unsigned char*)currentBlock < heap + HEAP_SIZE){
        printf("%s at %p, size %d \n", currentBlock->free ? "Free": "Used", currentBlock, currentBlock->size);

        if(currentBlock->free){
            for(int i = 0; i < currentBlock->size; i += 8){
                output[index] = initial_free;
                index++;
            }
            initial_free++;
        }else{
            for(int i = 0; i < currentBlock->size; i+= 8){
                output[index] = initial_used;
                index++;
            }
            initial_used++;
        }

        
        currentBlock = (memoryBlockHeader*)(((unsigned char*)currentBlock + currentBlock->size + sizeof(memoryBlockHeader)));
        
        
    }
    output[index] = '\0';
    printf("%s\n", output);

}

void printFreeList() {
    printf("Free List\n");

    memoryBlockHeader* currentBlock = freeListHead;
    
    if (currentBlock == NULL) {
        printf("No free blocks available.\n");
        return;
    }
    while (currentBlock != NULL) {  // Iterate through the free list
        if (currentBlock->free == 1) {
            printf("Free block at %p, size %d\n", (void*)currentBlock, currentBlock->size);
        }
        currentBlock = currentBlock->next;  // Move to the next block in the list
    }
}



void* duMalloc(int size) {
    if (size % 8 != 0) {
        size += (8 - size % 8);
    }
    int totalSize = size + sizeof(memoryBlockHeader);



    memoryBlockHeader* currentBlock = freeListHead;
    memoryBlockHeader* previousBlock = NULL;
    memoryBlockHeader* bestBlock = NULL;
    size_t bestSize = HEAP_SIZE + 1;

    
    
    while (currentBlock != NULL) {
        if (allocationStrategy == FIRST_FIT && currentBlock->size >= totalSize) {
            
            bestBlock = currentBlock;
            break;
        } else if (allocationStrategy == BEST_FIT && currentBlock->size >= totalSize && currentBlock->free == 1) {
            
            if (currentBlock->size < bestSize) {
                bestSize = currentBlock->size;
                bestBlock = currentBlock;
                
            }
        }
        
        previousBlock = currentBlock;
        currentBlock = currentBlock->next;
    }
    
    if (bestBlock == NULL) return NULL;

    //Finding previous block for best fit
    if(allocationStrategy == BEST_FIT && previousBlock != NULL){
        previousBlock = freeListHead;
        while(previousBlock->next != bestBlock && previousBlock->next != NULL){
            previousBlock = previousBlock->next;
        }
    }

    if (bestBlock->size == totalSize) {
        // Exact fit case, just remove bestBlock from the free list
        if (previousBlock == NULL) {
            freeListHead = bestBlock->next;
        } else {
            previousBlock->next = bestBlock->next;
        }
    } else {
        // Partial allocation, create a new free block
        memoryBlockHeader* newBlock = (memoryBlockHeader*)((unsigned char*)bestBlock + totalSize);
        newBlock->size = bestBlock->size - totalSize;
        newBlock->next = bestBlock->next;
        newBlock->free = 1;
      
        
        if (previousBlock == bestBlock || previousBlock == NULL) {
            
            freeListHead = newBlock;
        } else {
            
            previousBlock->next = newBlock;
        }

        bestBlock->size = size;
    }

    bestBlock->next = NULL;
    bestBlock->free = 0;
    
    
    return (void*)((unsigned char*)bestBlock + sizeof(memoryBlockHeader));
}

void duFree(void* ptr) {
    if (ptr == NULL) {
        return; // Check for null pointer
    }

    // Adjust pointer back to the memoryBlockHeader
    memoryBlockHeader* block = (memoryBlockHeader*)((unsigned char*)ptr - sizeof(memoryBlockHeader));
    block->free = 1; // Mark the block as free

    
    memoryBlockHeader* currentBlock = freeListHead;
    
    // Insert at the beginning if the free list is empty or block is smaller than the head
    if (currentBlock == NULL || currentBlock > block) {
        block->next = freeListHead;  
        freeListHead = block; 
        
        return;
    }

    // Find the position to insert the block in the free list
    while (currentBlock->next != NULL && currentBlock->next < block) {
        
        currentBlock = currentBlock->next;
    }
    
    // Insert the block into the free list
    block->next = currentBlock->next;

    currentBlock->next = block;
}




void duManagedInitMalloc(int searchType){
    duInitMalloc(searchType);
    for(int i = 0; i < HEAP_SIZE / 8; i++){
        managed_list[i] = NULL;
    }
    managed_size = 0;
}

void** duManagedMalloc(int size){
   memoryBlockHeader* block = duMalloc(size);
    if (block == NULL) {
        return NULL;  
    }
    if (managed_size >= HEAP_SIZE / 8) {
        return NULL;
    }
    block->managedIndex = managed_size;
    managed_list[managed_size] = block;
    managed_size++;

    return (void**) &managed_list[managed_size - 1];
}

void printManagedList() {
    printf("Managed Memory Dump\n");
    for (int i = 0; i < managed_size; i++) {
        if (managed_list[i] != NULL) {
            printf("ManagedList[%d] = %p\n", i, (void*)managed_list[i]);
        } else {
            printf("ManagedList[%d] = (nil)\n", i);
        }
    }
}


void duManagedFree(void** mptr){
    memoryBlockHeader* block = *mptr;
    if (block == NULL) return;
    managed_list[block->managedIndex] = NULL;
    duFree(block);
    *mptr = NULL;

}




void duMemoryDump(){
    printf("MEMORY DUMP\n");
    printMemoryBlockInfo();   
    printFreeList();
    printManagedList();
}