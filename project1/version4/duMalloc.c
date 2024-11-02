#include <stdio.h>
#include <string.h>
#include "dumalloc.h"

#define HEAP_SIZE (128*8)


int allocationStrategy;
unsigned char heap[3][HEAP_SIZE];
int currentHeap = 0;




typedef struct memoryBlockHeader {
    int free; // 0 - used, 1 = free
    int size; // size of the reserved block
    int managedIndex; // the unchanging index in the managed array
    int survivalAmt; // the number of times the block has moved between young heaps
    struct memoryBlockHeader* next;
} memoryBlockHeader;

memoryBlockHeader* freeListHead[3];

memoryBlockHeader* managed_list[HEAP_SIZE / 8];
int managed_size;

void duInitMalloc(int strategy){
    allocationStrategy = strategy;

    for(int i =0; i<HEAP_SIZE; i++){
        heap[currentHeap][i] = 0;
    }
    memoryBlockHeader* currentBlock = (memoryBlockHeader*)heap[currentHeap];

    currentBlock->size = HEAP_SIZE - sizeof(memoryBlockHeader);
    currentBlock->next = NULL;
    currentBlock->free = 1;
    currentBlock->survivalAmt = 0;
    freeListHead[currentHeap] = currentBlock;
    memoryBlockHeader* otherBlock = (memoryBlockHeader*)heap[2];

    otherBlock->size = HEAP_SIZE - sizeof(memoryBlockHeader);
    otherBlock->next = NULL;
    otherBlock->free = 1;
    otherBlock->survivalAmt = 0;
    freeListHead[2] = otherBlock;
}

void printMemoryBlockInfo(int heapNum){
    memoryBlockHeader* currentBlock = (memoryBlockHeader*)heap[heapNum];
    char output[HEAP_SIZE / 8];
    int index = 0;
    char initial_free = 'a';
    char initial_used = 'A';
    
    while((unsigned char*)currentBlock < heap[heapNum] + HEAP_SIZE){
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

void printFreeList(int heapNum) {
    printf("Free List\n");

    memoryBlockHeader* currentBlock = freeListHead[heapNum];
    
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



void* duMalloc(int size, int heapNum) {
    if (size % 8 != 0) {
        size += (8 - size % 8);
    }
    int totalSize = size + sizeof(memoryBlockHeader);

    

    memoryBlockHeader* currentBlock = freeListHead[heapNum];
    memoryBlockHeader* previousBlock = NULL;
    memoryBlockHeader* bestBlock = NULL;
    size_t bestSize = HEAP_SIZE + 1;

    // Print the initial size of the current free block
    

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
        previousBlock = freeListHead[currentHeap];
        while(previousBlock->next != bestBlock && previousBlock->next != NULL){
            previousBlock = previousBlock->next;
        }
    }

    if (bestBlock->size == totalSize) {
        // Exact fit case, just remove bestBlock from the free list
        if (previousBlock == NULL) {
            freeListHead[heapNum] = bestBlock->next;
        } else {
            previousBlock->next = bestBlock->next;
        }
    } else {
        // Partial allocation, create a new free block
        memoryBlockHeader* newBlock = (memoryBlockHeader*)((unsigned char*)bestBlock + totalSize);
        newBlock->size = bestBlock->size - totalSize;
        newBlock->next = bestBlock->next;
        newBlock->free = 1;
        newBlock->survivalAmt = 0;

        if (previousBlock == bestBlock || previousBlock == NULL) {
            freeListHead[heapNum] = newBlock;
        } else {
            previousBlock->next = newBlock;
        }

        bestBlock->size = size;
    }

    bestBlock->next = NULL;
    bestBlock->free = 0;
    bestBlock->survivalAmt = 0;
    
    return (void*)((unsigned char*)bestBlock + sizeof(memoryBlockHeader));
}



void duFree(void* ptr) {
    if (ptr == NULL) {
        return; // Check for null pointer
    }

    // Adjust pointer back to the memoryBlockHeader
    memoryBlockHeader* block = (memoryBlockHeader*)((unsigned char*)ptr - sizeof(memoryBlockHeader));
    block->free = 1; // Mark the block as free

    int tempHeapNum = ((unsigned char*)ptr >= heap[2] && (unsigned char*)ptr <= (heap[2] + HEAP_SIZE)) ? 2 : currentHeap;
    memoryBlockHeader* currentBlock = freeListHead[tempHeapNum];
    
    // Insert at the beginning if the free list is empty or block is smaller than the head
    if (currentBlock == NULL || currentBlock > block) {
        block->next = freeListHead[tempHeapNum];  
        freeListHead[tempHeapNum] = block; 
        
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
   memoryBlockHeader* block = duMalloc(size, currentHeap);
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
            // Move back to the header to access survivalAmt
            memoryBlockHeader* blockHeader = (memoryBlockHeader*)((unsigned char*)managed_list[i] - sizeof(memoryBlockHeader));
            printf("ManagedList[%d] = %p, Has been moved %d times\n", i, (void*)managed_list[i], blockHeader->survivalAmt);
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
    printf("Current Heap: %d\n", currentHeap);
    printMemoryBlockInfo(currentHeap);   
    printFreeList(currentHeap);

    printf("Old Heap\n");
    printMemoryBlockInfo(2);   
    printFreeList(2);
    printManagedList();
}

void minorCollection(){
    
    int otherHeap = currentHeap == 0 ? 1 : 0;

    
    void* pointer = heap[otherHeap];
    memoryBlockHeader* previousBlock = (memoryBlockHeader*)pointer;
    memoryBlockHeader* block;
    for(int i = 1; i < managed_size; i++){
        
        if(managed_list[i] != NULL && !((unsigned char*)managed_list[i] > heap[2] && (unsigned char*)managed_list[i] < heap[2] + HEAP_SIZE)){
            memoryBlockHeader* block = (void*) managed_list[i] - sizeof(memoryBlockHeader);
            block->survivalAmt++;
            if(block->survivalAmt == 3){
                
                void* oldGenLoc = duMalloc(block->size, 2); // Allocate in old generation (heap 2)
                
                memcpy(oldGenLoc - sizeof(memoryBlockHeader), block, block->size + sizeof(memoryBlockHeader));
                
                managed_list[i] = (memoryBlockHeader*)oldGenLoc;
                
            }
            else{
                previousBlock->next = block;
                
                block->next = NULL;
                
                
                memcpy(pointer, block, block->size + sizeof(memoryBlockHeader));
                managed_list[i] = pointer + sizeof(memoryBlockHeader);
                previousBlock = block;
                pointer += block->size + sizeof(memoryBlockHeader);
            }
        }
    }
    memoryBlockHeader* newBlock = (memoryBlockHeader*)pointer;
    previousBlock->next = newBlock;
    newBlock->size = HEAP_SIZE - ((unsigned char*)pointer - heap[otherHeap]);
    newBlock->next = NULL;
    newBlock->free = 1;
    newBlock->survivalAmt = 0;
    freeListHead[otherHeap] = newBlock;
    currentHeap = otherHeap;
}

void majorCollection() {
    memoryBlockHeader* currentBlock = freeListHead[2];
    memoryBlockHeader* freeBlock = NULL;

    //Finds first free block in old generation
    while (currentBlock != NULL) {
        if (currentBlock->free) {
            
            freeBlock = currentBlock;
            break;
        }
        currentBlock = currentBlock->next;
    }   

    // Ensure nextBlock is correctly calculated
    memoryBlockHeader* nextBlock = (memoryBlockHeader*)((unsigned char*)freeBlock + freeBlock->size + sizeof(memoryBlockHeader));

    while ((unsigned char*)nextBlock < heap[2] + HEAP_SIZE) {
        if (nextBlock->free) {
            // Combine free blocks
            freeBlock->size += nextBlock->size + sizeof(memoryBlockHeader);
            freeBlock->next = nextBlock->next;
            
        } else {
            // Move the used block to the free block
            
            memcpy(freeBlock, nextBlock, nextBlock->size + sizeof(memoryBlockHeader));
            // Update the size of the block being moved
            freeBlock->size = nextBlock->size; // Ensure size is set correctly

            // Update the managed list
            managed_list[nextBlock->managedIndex] = freeBlock;

            // Create a new free block
            memoryBlockHeader* newFreeBlock = (memoryBlockHeader*)((unsigned char*)freeBlock + freeBlock->size + sizeof(memoryBlockHeader));
            newFreeBlock->free = 1;
            newFreeBlock->size = (unsigned char*)nextBlock + nextBlock->size - (unsigned char*)newFreeBlock; // Correctly calculate size
            newFreeBlock->next = nextBlock->next;
            newFreeBlock->survivalAmt = 0;

            // Update the free list
            freeBlock->next = newFreeBlock;
            freeBlock = newFreeBlock;
        }
        
        // Move nextBlock to the next potential block
        nextBlock = (memoryBlockHeader*)(((unsigned char*)nextBlock) + nextBlock->size + sizeof(memoryBlockHeader));
    }


}
