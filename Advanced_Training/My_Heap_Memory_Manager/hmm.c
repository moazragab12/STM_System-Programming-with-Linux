/*******************************Author:Moaz Ragab ***********************************************/
/*******************************Date:15 April 2024 *********************************************/
/*******************************V:02 **********************************************************/

#include <stdio.h>
#include <unistd.h>
#include "hmm.h"
#include <stdlib.h>
#include <string.h>

char *heap = NULL;
int first_sbrk = 1;
node *head = NULL;
/*
 * Enable if you want to replace libc malloc/free
 */
#if 1
void *malloc(size_t size)
{
  return heap_allocator(size);
}

void free(void *ptr)
{
  heap_free(ptr);
}

void *calloc(size_t nmemb, size_t size)
{
  return heap_calloc(nmemb, size);
}

void *realloc(void *ptr, size_t size)
{
  return heap_realloc(ptr, size);
}
#endif

void split(node *nodes, size_t size)
{
  if ((node_size + size) <= nodes->size)
  {
    node *temp = (node *)((void *)nodes + size + node_size);
    temp->size = (nodes->size) - size - node_size;

    temp->free = yes;
    temp->next = nodes->next;
    temp->prev = nodes;

    nodes->next = temp;
    nodes->size = size;
    // to protect against the case of the last block
    if (NULL != temp->next)
    {
      temp->next->prev = temp;
    }
  }
  nodes->free = no;
}

void Init_Heap(size_t size)
{
  heap = sbrk(size);

  if (heap == (void *)-1)
  {
    perror(" sbrk failed ");
  }
  else if (heap == NULL)
  {
    perror(" sbrk failed ");
  }
  first_sbrk = 0;

  // allocate free space
  head = (node *)heap;
  head->prev = NULL;
  head->size = size - node_size;
  head->next = NULL;
  head->free = yes;
}

void *heap_allocator(size_t size)
{
  size = (((size + 7) / Align) * Align);

  // If it's the first allocation, initialize the heap
  if (first_sbrk)
  {
    Init_Heap((((size + node_size) / INIT_SPACE) + 1) * INIT_SPACE);
    first_sbrk = 0;
    split(head, size);
    return (void *)(head + 1);
  }
  else
  {
    node *current = head;
    node *best_fit = NULL;
    // Traverse the linked list to find a  the best suitable free block
    while (current->next != NULL)
    {
      if (current->free == yes && current->size >= size)
      {

        if (current->size < size + node_size)
        {
          // if we find exactly the same size, we don't need to  complete the list to save time
          current->free = no;
          return (void *)(current + 1);
        }
        else
        {
          // if we find a block that is larger than the requested size, we need to find the best fit
          if (NULL == best_fit) // if it is the first block we find that is larger than the requested size
            best_fit = current;
          else if (current->size <= best_fit->size) // if we find a block that is larger than the requested size and smaller than the previous best fit
            best_fit = current;
        }
      }
      current = (node *)current->next;
    }

    if (NULL != best_fit)
    {
      if (best_fit->size >= size + node_size)
        split(best_fit, size);
      return (void *)(best_fit + 1);
    }
    else
    {
      best_fit = current;
      // If no suitable free block is found, allocate more memory from the heap
      long long sz =
          ((((size + node_size) / INIT_SPACE) + 1) * INIT_SPACE);
      void *check;
      while (sz > size_t_Max)
      {
        // Allocate the maximum size_t value if the requested size
        // is too large
        check = sbrk(size_t_Max);
        sz -= size_t_Max;
        if (check == (void *)-1)
        {
          perror(" sbrk failed ");
        }
      }
      check = sbrk(sz);
      if (check == (void *)-1)
      {
        perror(" sbrk failed ");
      }

      if (best_fit->free == no)
      {
        // Allocate a new block at the end of the linked list
        best_fit->next =
            (void *)best_fit + best_fit->size + node_size;
        best_fit->next->prev = best_fit;
        best_fit = best_fit->next;
        best_fit->free = yes;
        // Calculate the size of the new block
        best_fit->size =
            ((((size + node_size) / INIT_SPACE) +
              1) *
             INIT_SPACE) -
            node_size;
        // set the block as last block
        best_fit->next = NULL;
      }
      else
      {
        best_fit->size +=
            ((((size + node_size) / INIT_SPACE) + 1) * INIT_SPACE);
      }
      split(best_fit, size);
    }
    return (void *)(best_fit + 1);
  }
}

void merge(node *nodes)
{
  if (nodes->next != NULL && (nodes->next)->free == yes)
  {
    // merge the next block with the current block
    nodes->free = yes;
    if (nodes->next->next != NULL)
      ((nodes->next)->next)->prev = nodes;

    nodes->size += (nodes->next)->size + node_size;
    nodes->next = (nodes->next)->next;
  }

  if (nodes->prev != NULL && (nodes->prev)->free == yes)
  {
    // merge the previous block with the current block
    nodes->free = yes;
    (nodes->prev)->size += nodes->size + node_size;
    (nodes->prev)->next = nodes->next;
    // to protect against the case of the last block
    if (nodes->next != NULL)
      nodes->next->prev = nodes->prev;
    nodes = nodes->prev;
  }
  // check if the last block is free and its size is larger than the
  // Boundry_free_Size
  if (nodes->next == NULL && nodes->free == yes && nodes->size >= Boundry_free_Size)
  {
    long long temp =
        (nodes->size / Boundry_free_Size) * Boundry_free_Size;

    // release the memory to the kernel
    if (temp == nodes->size)
    {
      nodes->prev->next = NULL;

      void *check = sbrk(-temp - node_size);
      if (check == (void *)-1)
      {
        perror(" sbrk failed ");
      }
    }
    else
    {
      void *check = sbrk(-temp);
      if (check == (void *)-1)
      {
        perror(" sbrk failed ");
      }
      nodes->size -= temp;
    }
  }

  return;
}

void heap_free(void *ptr)
{
  if (ptr == NULL)
  {
    return;
  }
  if ((char *)ptr < (char *)heap)
  {
    return;
  }
  int flg = 0;
  node *temp1 = (node *)head;

  // Search for the node corresponding to the given pointer to check if
  // it is in my node
  while ((char *)temp1 >= (char *)heap && temp1->next != NULL)
  {
    if (temp1 + 1 == ptr)
    {
      flg = 1;
      break;
    }
    temp1 = (node *)temp1->next;
  }
  if (flg == 0)
  {
    return;
  }

  node *temp = (node *)ptr - 1;
  temp->free = 1;

  merge(temp);
}

void *heap_calloc(size_t nmemb, size_t size)
{

  long int totsize = nmemb * size;
  if (0 == nmemb || 0 == size)
    return NULL;
  if (nmemb * size < 0)
  {
    perror("Calloc failed the size is too large");
    return NULL;
  }
  // allocate the memory
  void *ptr = heap_allocator(totsize);
  if (ptr != NULL)
  {
    // initialize the memory to zero
    memset(ptr, 0, totsize);
  }
  return ptr;
}

void *heap_realloc(void *ptr, size_t size)
{
  void *newptr = NULL; // Initialize new pointer to NULL

  if (NULL == ptr)
  {
    newptr = malloc(size); // Allocate memory if ptr is NULL
  }
  else if (0 == size)
  {
    free(ptr); // Free memory if size is zero
  }
  else
  {
    if (size <= ((node *)(ptr - node_size))->size)
    {
      newptr = ptr; // Return ptr if size is smaller or equal
                    // than the current allocated size
    }
    else
    {
      newptr = malloc(size); // Allocate new memory block
      if (NULL != newptr)
      {
        memcpy(newptr, ptr, ((node *)(ptr - node_size))->size); // Copy data  to new memory block
        free(ptr);
      }
    }
  }
  return newptr; // Return pointer to reallocated memory
                 // block
}
