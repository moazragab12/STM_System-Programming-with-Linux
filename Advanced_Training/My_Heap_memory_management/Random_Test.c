
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "hmm.h"
#define NUM_ALLOCS 10000
#define MAX_SIZE 1024
#define MAX_ITERATIONS 1000
void random_alloc_free_test() {
    srand((unsigned int)time(NULL));

    void* pointers[NUM_ALLOCS] = {NULL};

    for (int i = 0; i < MAX_ITERATIONS; ++i) {
        int index = rand() % NUM_ALLOCS;
        if (pointers[index] == NULL) {
            // Allocate memory
            size_t size = (size_t)(rand() % MAX_SIZE) + 1;
            pointers[index] = heap_allocator(size);
            if (pointers[index] != NULL) {

                printf("Allocated memory of size %zu at address %p\n", size, pointers[index]);

            } else {
                fprintf(stderr, "Allocation failed for size %zu\n", size);
            }
        } else {
            // Free memory
            printf("Freeing memory at address %p\n", pointers[index]);
            heap_free(pointers[index]);
            pointers[index] = NULL;
        }
    }

    // Free remaining allocated memory
    for (int i = 0; i < NUM_ALLOCS; ++i) {
        if (pointers[i] != NULL) {
            printf("Freeing remaining memory at address %p\n", pointers[i]);
            heap_free(pointers[i]);
            pointers[i] = NULL;
        }
    }
}

int main() {


        printf("Starting random allocation and deallocation test...\n");
        random_alloc_free_test();
        printf("Test complete.\n");

    return 0;
}
