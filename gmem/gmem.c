#include <stdio.h>
#include "gmem.h"

#define HEAP_SIZE 2048
#define WORD_SIZE sizeof(unsigned int)

// Global memory
static void * mem[HEAP_SIZE];

// Memory blocks
struct block * p; // empty

// Initialize GMem
void init() {
  printf("init\n");
  p = (struct block *) mem;
  p->size = HEAP_SIZE;
  p->next = p;
}

// Allocate a block of bytes
void * gmalloc(unsigned int size) {
  void * ptr = p;
  unsigned int s = size + (size % WORD_SIZE) + WORD_SIZE;
  printf("gmalloc: %u wanted\n", size);
  while (p->next->size < s) {
    printf("p->next->size = %u, s = %u", p->next->size, s);
    // TODO try merge
    p = p->next;
    if (p == ptr) return (void *) -1;
  }
  printf("gmalloc: %u free\n", p->next->size);
  ptr = p->next;
  if (ptr->size == s) {
    p = p->next->next;
  } else {
    p->next = ptr + s;
    p = p->next;
    printf("p->size = %u, p->next = %p\n", p->size, p->next);
    p->size = (unsigned int) ((struct block *) ptr)->size - s; // FUCK
    printf("p->size = %u, p->next = %p\n", p->size, p->next);
    p->next = (struct block *) ((struct block *) ptr)->next;
    printf("p->size = %u, p->next = %p\n", p->size, p->next);
  }
  *((unsigned int *) ptr) = s;
  return ptr + WORD_SIZE;
}

// Free a previously allocated block
void gfree(void * ptr) {
  struct block * b = ptr - WORD_SIZE;
  unsigned int s = *((unsigned int *) b);
  printf("gfree: %u\n", s - (unsigned int) WORD_SIZE);
  b->size = s;
  // TODO put back into linked list
}

// Test entry point
int main (int argc, char * argv[]) {
  void * ptr;
  init();
  ptr = gmalloc(4); gfree(ptr);
  ptr = gmalloc(8); gfree(ptr);
  ptr = gmalloc(15); gfree(ptr);
  ptr = gmalloc(16); gfree(ptr);
  ptr = gmalloc(23); gfree(ptr);
  ptr = gmalloc(42); gfree(ptr);

  return 0;
}
