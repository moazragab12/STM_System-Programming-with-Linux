/**
 * @file custom_heap_manager.h
 * @brief Custom memory management using sbrk system call.
 *
 * This file provides functions for custom memory allocation, deallocation,
 * and reallocation using the sbrk system call to manipulate the program break.
 */

#include <stdio.h>
#include <unistd.h>

/** @def node_size
 *  @brief Size of the node structure.
 *
 *  node_size represents the size of the node structure used to manage memory blocks.
 */
#define node_size sizeof(node)

/** @def page
 *  @brief Size of a page in bytes.
 *
 *  page represents the size of a page in bytes, used for initializing the heap.
 */
#define page 4096

/** @def INIT_SPACE
 *  @brief Initial space allocated by sbrk.
 *
 *  INIT_SPACE is the initial space allocated by sbrk when the heap is initialized.
 */
#define INIT_SPACE (page *33)

/** @def Boundry_free_Size
 *  @brief Boundary size for releasing memory to kernel.
 *
 *  Boundry_free_Size is the threshold size for releasing memory to the kernel when deallocating.
 */
#define Boundry_free_Size (page *32)

/** @def yes
 *  @brief Flag indicating a free memory block.
 *
 *  yes is used to indicate a free memory block.
 */
#define yes 1

/** @def no
 *  @brief Flag indicating an allocated memory block.
 *
 *  no is used to indicate an allocated memory block.
 */
#define no 0

/** @def Align
 *  @brief Alignment for memory allocation.
 *
 *  Align is used to ensure memory is aligned properly for data structures.
 */
#define Align 8

/** @def size_t_Max
 *  @brief Maximum value for size_t.
 *
 *  size_t_Max is the maximum value for the size_t data type.
 */
#define size_t_Max 4294967295

/**
 * @struct node
 * @brief Structure representing a memory block.
 *
 * This structure represents a memory block in the custom heap.
 */
typedef struct node {
    struct node *prev;  /**< Pointer to the previous memory block. */
    struct node *next;  /**< Pointer to the next memory block. */
    long long int size;        /**< Size of the memory block. */
    char free;          /**< Flag indicating if the block is free or not. */
} node;

/**
 * @brief Merges adjacent free memory blocks.
 *
 * This function merges adjacent free memory blocks to prevent fragmentation.
 *
 * @param nodes Pointer to the memory block to merge.
 */
void merge(node *nodes);

/**
 * @brief Initializes the custom heap.
 *
 * This function initializes the custom heap by calling the sbrk system call.
 *
 * @param size Size of the initial heap space.
 */
void Init_Heap(size_t size);
/**
 * @brief Splits a memory block into two parts.
 *
 * If the size of the memory block is larger than the requested size, it splits the block into two parts.
 *
 * @param nodes Pointer to the memory block to split.
 * @param size Size of the memory block to allocate.
 */
void split(node *nodes, size_t size);

/**
 * @brief Allocates memory on the custom heap.
 *
 * This function allocates memory on the custom heap using the sbrk system call.
 *
 * @param size Size of the memory block to allocate.
 * @return Pointer to the allocated memory block.
 */
void *heap_allocator(size_t size);

/**
 * @brief Frees memory allocated on the custom heap.
 *
 * This function frees memory allocated on the custom heap.
 *
 * @param ptr Pointer to the memory block to free.
 */
void heap_free(void *ptr);

/**
 * @brief Allocates memory on the custom heap and initializes it to zero.
 *
 * This function allocates memory on the custom heap and initializes it to zero.
 *
 * @param nmemb Number of elements.
 * @param size Size of each element.
 * @return Pointer to the allocated memory block.
 */
void *heap_calloc(size_t nmemb, size_t size);

/**
 * @brief Reallocates memory on the custom heap.
 *
 * This function reallocates memory on the custom heap.
 *
 * @param ptr Pointer to the previously allocated memory block.
 * @param size New size of the memory block.
 * @return Pointer to the reallocated memory block.
 */
void *heap_realloc(void *ptr, size_t size);