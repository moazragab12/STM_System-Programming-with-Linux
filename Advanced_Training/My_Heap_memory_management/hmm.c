#include<stdio.h>
#include <unistd.h>
#include"hmm.h"
#define page 4096


char *upper_sbrk;
long int lower_sbrk = 0;
char *heap = NULL;
int first_sbrk = 1;
node *head = NULL;
node *tail = NULL;


void split(node * current, node * previos, int size)
{
    node *temp;
    int s = current->size;
    char *ptr = (char *) current->next;
    current->free = 0;
    current->size = size;
    current->next = (char *) ((char *) current + size + sizeof(node));
    current->prev = (char *) previos;
    temp = (node *) current->next;
    temp->size = s - size - (int) sizeof(node);
    temp->free = 1;
    temp->prev = (char *) current;
    temp->next = (char *) ptr;
    if(temp->next>upper_sbrk)
    {     sbrk((__intptr_t) 16 * page);
	    upper_sbrk = (char *) sbrk(0) - 1;
    }
    ((node *) temp->next)->prev = (char *) temp;

//edit tail

if(current>tail && current->free==0)
   { tail = current;}
   
    return;


}



void *heap_allocator(size_t size)
{
    if (first_sbrk) {
	heap = sbrk((__intptr_t) 9 * page);
	upper_sbrk = (char *) sbrk(0) - 1;

	first_sbrk = 0;
    }
    static int first = 1;
    if (first) {
	head = (node *) heap;
	head->prev = NULL;
	head->size = (int) sizeof(node) + (int) size;
	head->next = heap + sizeof(node) + size;
	first = 0;
	head->free = 0;
	tail = head;
	return (void *) (head + 1);

    } else {
	node *current = (node *) head->next;
	node *previos = head;
	while (current->next != NULL) {
	    if (current->free == 1 && current->size >= (int) size) {
		if (current->size > (int) size)
		    split(current, previos, (int) size);
		else {
		    current->free = 0;
        if(current>tail && current->free==0)
	tail = current;
		  
		 

		}
    if(current>tail && current->free==0)
	tail = current;
		return (void *) (current + 1);
	    }
	    previos = current;
	    current = (node *) current->next;

	}
	while ((char *) tail+tail->size + size + 2*sizeof(node) >= upper_sbrk) {
	    sbrk((__intptr_t) 8 * page);
	    upper_sbrk = (char *) sbrk(0) - 1;

	}
	current->free = 0;
	current->size = (int) size;
	current->next = (char *) ((char *) current + size + sizeof(node));
	current->prev = (char *) previos;
if(current>tail && current->free==0)
	{tail = current;}
	//  tail->free=1;
	//  tail->next=NULL;
	//    tail->prev=(char *)current;
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
	nodes->next = ((node *) nodes->next)->next;
	((node *) (nodes->next))->prev = (char *) nodes;
   if(tail==nodes)
   { node *temp = nodes;
     while (temp->prev != NULL && ((node *) temp->prev)->free == 1 )
   {
   
    temp = (node *) temp->prev;
   }
   
tail = (node *) temp->prev;
   }
    }
    while (nodes->prev != NULL && ((node *) nodes->prev)->free == 1) {
	nodes->free = 1;

  if(tail==nodes)
   { node *temp = nodes;
     while (temp->prev != NULL && ((node *) temp->prev)->free == 1 )
   {
    
    temp = (node *) temp->prev;
   }
   tail = (node *) temp->prev;

   }


	((node *) (nodes->prev))->size += nodes->size + (int) sizeof(node);
	((node *) (nodes->prev))->next = nodes->next;
	nodes = (node *) nodes->prev;
	try->prev = (char *) nodes;

  
    }


while((__intptr_t)(upper_sbrk)>13*page+(__intptr_t)tail+tail->size+node_size)
 {
    sbrk(-12*page);
    upper_sbrk = (char *) sbrk(0) - 1;
 }
    return;
}

void heap_free(void *ptr)
{
    node *temp = (node *) ptr - 1;
    temp->free = 1;
  
	merge(temp);
    



}


void *heap_calloc(size_t nmemb, size_t size)
{
    return NULL;
}

void *heap_realloc(void *ptr, size_t size)
{
    return NULL;
}
/*
int main(void)
{
    int *ptr;
    for (int i = 0; i < 3; i++) {
	ptr = (int *) heap_allocator( page);



    }

    heap_free(ptr);
    int *ptr1 = (int *) heap_allocator(3000);
    int *ptr2 = (int *) heap_allocator(500);
 int *ptr3=   (int *) heap_allocator(15 * page);
    heap_allocator(500);
   heap_free( heap_allocator(1000));
    heap_free(ptr1);
    heap_free(ptr2);
   heap_free(ptr3);
    int *ptr11[10];
        for (int i = 0; i < 10; i++) {
	ptr11[i] = (int *) heap_allocator( 3*page);
    }
    for(int i=0;i<10;i++)
    { heap_free(ptr11[i]);
    }
        node *temp = (node *) head;
    while (temp->next != NULL) {
	if (temp->free == 1) {
	    printf
		("free size     %d , Address of page %p, free stat %d , prev %p , next %p \n",temp->size, temp, temp->free, temp->prev, temp->next);
	}

	else {
	    printf
		("allocated size %d, Address of page %p, free stat %d , prev %p , next %p \n",
		 temp->size, temp, temp->free, temp->prev, temp->next);
	}
	temp = (node *) temp->next;

    }
    printf("upper sbrk %p\n", upper_sbrk);

    printf("lower sbrk %ld\n", lower_sbrk);

    printf("diff %ld\n", (upper_sbrk - heap) / page);
    printf("tail %p\n", tail);

    return 0;
}
*/