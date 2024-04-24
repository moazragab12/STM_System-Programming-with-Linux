#include<stdio.h>
#include <unistd.h>
#include"hmm.h"

#include <stdlib.h>
#include <string.h>
#define page 4096

static int ctr = 0;

char *upper_sbrk;
long int lower_sbrk = 0;
char *heap = NULL;
int first_sbrk = 1;
node *head = NULL;
node *tail = NULL;
/* Enable if you want to replace libc malloc/free */
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

void split(node * current, node * previos, int size)
{
    node *temp;
    int s = (int) current->size;
    char *ptr = (char *) current->next;
    current->free = 0;
    current->size = size;
    current->next = (char *) ((char *) current + size + sizeof(node));
    current->prev = (char *) previos;
    //printf("IN SPLIT:PREV of current PTR: %p ,current ptr :%p , NEXT of current  PTR after spilting: %p, NEXT BEFORE SPLIT:%p \n",current->prev,current,current->next,ptr);

    if (s - size - (int) sizeof(node) >= 0) {
	temp = (node *) current->next;
	while ((char *) temp > (char *) upper_sbrk) {
	    sbrk(16 * page);
	    upper_sbrk = (char *) sbrk(0) - 1;
	}
	temp->size = s - size - (int) sizeof(node);
	temp->free = 1;
	temp->prev = (char *) current;
	temp->next = (char *) ptr;
	//      printf("IN SPLIT if1:PREV of temp PTR: %p ,temp ptr :%p , NEXT of temp PTR: %p \n",temp->prev,temp,temp->next);
	if (temp->next > upper_sbrk) {
	    sbrk(16 * page);
	    upper_sbrk = (char *) sbrk(0) - 1;
	}

	if (((node *) temp->next)->prev != NULL
	    && ((node *) temp->next)->prev < upper_sbrk
	    && ((node *) temp->next)->prev > heap)
	    ((node *) temp->next)->prev = (char *) temp;
    } else {
	current->size = s;
	current->next = (char *) ptr;
	//      printf("IN SPLIT couldnot merge:PREV of current PTR: %p ,current ptr :%p , NEXT of current PTR: %p \n",current->prev,current,current->next);

    }

//edit tail

    if (current > tail && current->free == 0) {
	tail = current;
    }

    return;


}



void *heap_allocator(size_t size)
{
    size = (((size + 7) / 8) * 8);

    if (first_sbrk) {
	heap = sbrk(9 * page);
	upper_sbrk = (char *) sbrk(0) - 1;

	first_sbrk = 0;
    }
    static int first = 1;
    if (first) {
	head = (node *) heap;
	tail = (node *) heap;
	head->prev = NULL;
	head->size = (int) sizeof(node) + (int) size;
	while ((int) sizeof(node) + (int) size + (char *) head >
	       (char *) upper_sbrk) {
	    sbrk((8 * page));
	    upper_sbrk = (char *) sbrk(0) - 1;
	}
	head->next = heap + sizeof(node) + size;
	first = 0;
	head->free = 0;
	//  printf("PREV of current PTR: %p ,current ptr :%p , NEXT of current PTR: %p \n",head->prev,head,head->next);
	return (void *) (head + 1);

    } else {
	node *current = (node *) head->next;
	node *previos = head;



	while ((char *) current->next + (node_size) < (char *) upper_sbrk
	       && current->next != NULL) {

	    if (current->free == 1 && current->size >= (int) size) {
		if (current->size > (int) size + node_size)
		    split(current, previos, (int) size);
		else {
		    current->free = 0;
		    if ((char *) current > (char *) tail
			&& current->free == 0)
			tail = current;



		}
		if (current > tail && current->free == 0)
		    tail = current;
		return (void *) (current + 1);
	    }
	    //testing
	    //      printf("PREV of previous PTR: %p ,previous ptr :%p , NEXT of previous PTR: %p \n",previos->prev,previos,previos->next);
	    previos = current;
	    if (previos == tail) {
		current = (node *) previos->next;
		break;
	    }
	    //printf("PREV of current PTR: %p ,current ptr :%p , NEXT of current PTR: %p, SIZE %d \n",current->prev,current,current->next,current->size);

	    current = (node *) current->next;
	    if (current > tail && NULL != current)
		tail = current;


	}

	while ((char *) tail + tail->size + size + 2 * sizeof(node) >=
	       upper_sbrk) {
	    sbrk(8 * page);
	    upper_sbrk = (char *) sbrk(0) - 1;

	}
	current->free = 0;
	current->size = (int) size;
	current->next = (char *) ((char *) current + size + sizeof(node));




	current->prev = (char *) previos;
	if (current > tail && current->free == 0) {
	    tail = current;
	}

	return (void *) (current + 1);


    }
}



void merge(node * nodes)
{
    node *try;
    try = (node *) nodes->next;

    while (nodes->next != NULL && ((node *) nodes->next)->free == 1) {
	nodes->free = 1;

	nodes->size += ((node *) nodes->next)->size + (int) sizeof(node);



	nodes->next =
	    node_size + (char *) ((node *) nodes->next) +
	    ((node *) nodes->next)->size;
// accessing null here
	if (nodes->next != NULL)
	    ((node *) (nodes->next))->prev = (char *) nodes;
	if (tail == nodes) {
	    node *temp = nodes;
	    while (temp->prev != NULL && ((node *) temp->prev)->free == 1) {

		temp = (node *) temp->prev;
	    }
	    if (temp->prev != NULL)
		tail = (node *) temp->prev;
	}
    }


    if (nodes->next == NULL) {
	((node *) nodes->prev)->next = NULL;
	sbrk(-nodes->size);
	upper_sbrk = sbrk(0);
    }


    while (nodes->prev != NULL && ((node *) nodes->prev)->free == 1) {
	//nodes->free = 1;

	if (tail == nodes) {
	    node *temp = nodes;
	    while (temp->prev != NULL && ((node *) temp->prev)->free == 1) {

		temp = (node *) temp->prev;
	    }
	    if (temp->prev != NULL)
		tail = (node *) temp->prev;

	}


	((node *) (nodes->prev))->size += nodes->size + (int) sizeof(node);
	if ((char *) nodes->next < (char *) upper_sbrk)
	    ((node *) (nodes->prev))->next = nodes->next;
	else
	    ((node *) (nodes->prev))->next = NULL;
	nodes = (node *) nodes->prev;
	if (((char *) try + node_size) < (char *) upper_sbrk)
	    try->prev = (char *) nodes;


    }


    while ((char *) (upper_sbrk) > 13 * page + (char *) tail + tail->size + node_size) {	//error accessing out of boundry if next is big
	if (((nodes->next) != NULL)
	    && ((node *) nodes->next)->next == NULL) {
	    nodes->next = NULL;
	    //    ((node *) nodes->prev)->next=NULL;
	    sbrk(-nodes->size);
	    upper_sbrk = sbrk(0);
	    if (nodes->prev != NULL)
		tail = (node *) nodes->prev;
	} else {
	    sbrk(-12 * page);

	    upper_sbrk = (char *) sbrk(0) - 1;

	}
    }

    return;
}

void heap_free(void *ptr)
{
    if (ptr == NULL) {
	return;
    }
    if ((char *) ptr < (char *) heap || (char *) ptr > (char *) upper_sbrk) {
	return;
    }
    int flg = 0;
    node *temp1 = (node *) head;


    while ((char *) temp1 < (char *) upper_sbrk
	   && (char *) temp1 >= (char *) heap && temp1->next != NULL) {
	if (temp1 + 1 == ptr) {
	    flg = 1;
	    break;
	}
	temp1 = (node *) temp1->next;
    }
    if (flg == 0) {
	return;
    }

    node *temp = (node *) ptr - 1;
    temp->free = 1;

    merge(temp);




}


void *heap_calloc(size_t nmemb, size_t size)
{
    long int totsize = nmemb * size;
    if (0 == nmemb || 0 == size)
	return NULL;
    if (nmemb * size < 0) {
	perror("Calloc failed the size is too large");
	return NULL;
    }

    void *ptr = heap_allocator(totsize);
    if (ptr != NULL) {
	memset(ptr, 0, totsize);
    }
    return ptr;
}

void *heap_realloc(void *ptr, size_t size)
{
    void *reValue;

    if (ptr == NULL) {
	return heap_allocator(size);
    }
    if (size == 0) {
	heap_free(ptr);
	return reValue;
    }
    if (size <= ((node *) (ptr - node_size))->size) {
	return ptr;
    }

    reValue = heap_allocator(size);

    memcpy(reValue, ptr, ((node *) (ptr - node_size))->size);
    heap_free(ptr);
    return reValue;
}


//
//
// int main(void)
// {
//
//     int *ptr[3];
//     for (int i = 0; i < 3; i++) {
//      ptr[i] = (int *) heap_allocator( page);
//
//
//
//     }
//      heap_free(ptr[1]);
//      heap_allocator(4086);
//      heap_allocator(10);
//
//
//     heap_free(ptr);
//     int *ptr1 = (int *) heap_allocator(3000);
//     int *ptr2 = (int *) heap_allocator(500);
//  int *ptr3=   (int *) heap_allocator(15 * page);
//     heap_allocator(500);
//    heap_free( heap_allocator(1000));
//     heap_free(ptr1);
//     heap_free(ptr2);
//    heap_free(ptr3);
//     int *ptr11[1000];
//         for (int i = 0; i < 1000; i++) {
//      ptr11[i] = (int *) heap_allocator( 4*page);
//     }
//     for(int i=100;i<1000;i++)
//     {
//         heap_free(ptr11[i]);
//     }
//     heap_allocator(page);
//         node *temp = (node *) head;
//     while (temp->next != NULL) {
//      if (temp->free == 1) {
//          printf
//              ("free size     %d , Address of page %p, free stat %d , prev %p , next %p \n",temp->size, temp, temp->free, temp->prev, temp->next);
//      }
//
//      else {
//          printf
//              ("allocated size %d, Address of page %p, free stat %d , prev %p , next %p \n",
//               temp->size, temp, temp->free, temp->prev, temp->next);
//      }
//      temp = (node *) temp->next;
//
//     }
//     printf("upper sbrk %p\n", upper_sbrk);
//
//     printf("lower sbrk %ld\n", lower_sbrk);
//
//     printf("diff %ld\n", (upper_sbrk - heap) / page);
//     printf("tail %p\n", tail);
//
//     return 0;
// }
//
