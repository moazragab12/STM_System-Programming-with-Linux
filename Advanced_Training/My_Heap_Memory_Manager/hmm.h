#include<stdio.h>
#include <unistd.h>


#define node_size sizeof(node)

typedef struct {
    char *prev;
    char *next;
   unsigned int size;
    int free;
} node;

void heap_free(void *ptr);
void * heap_allocator(size_t size);
void *heap_calloc(size_t nmemb, size_t size);
void *heap_realloc(void *ptr, size_t size);