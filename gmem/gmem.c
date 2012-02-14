#include <stdio.h>
#include "gmem.h"

#define HEAP_SIZE 2048
#define WORD_SIZE sizeof(unsigned int)
#define MIN_BLOCK sizeof(struct block)

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

// Max function
unsigned int max(unsigned int a, unsigned int b) {
  if (a > b) return a;
  return b;
}

// Allocate a block of bytes
void * gmalloc(unsigned int size) {
  if (size < 0 || size > HEAP_SIZE || p == 0) {
    return (void *) -1;
  }
  void * ptr = p;
  unsigned int s = max(MIN_BLOCK, size + (size % WORD_SIZE) + WORD_SIZE);
  printf("gmalloc: %u wanted, taking %u\n", size, s);
  while (p->next->size != s &&  p->next->size < s + sizeof(struct block)) {
    printf("p->next->size = %u, s = %u\n", p->next->size, s);
    if (p->next + p->next->size == p->next->next) {
      printf("merge\n");
      p->next->size += p->next->next->size;
      p->next->next = p->next->next->next;
    } else {
      printf("next\n");
      p = p->next;
      if (p == ptr) {
        printf("not enough memory\n");
        return (void *) -1;
      }
    }
  }
  printf("gmalloc: %u free\n", p->next->size);
  ptr = p->next;
  if (p->next->size == s) {
    if (p->next == p->next->next) {
      p = 0;
    } else {
      p->next = p->next->next;
      printf("setting p to %p, p->next is %p\n", p, p->next);
    }
  } else {
    p->next = ptr + s;
    p = p->next;
    printf("p->size = %u, p->next = %p\n", p->size, p->next);
    p->next = (struct block *) ((struct block *) ptr)->next;
    printf("p->size = %u, p->next = %p\n", p->size, p->next);
    p->size = (unsigned int) ((struct block *) ptr)->size - s;
    printf("p->size = %u, p->next = %p\n", p->size, p->next);
  }
  *((unsigned int *) ptr) = s;
  printf("allocated pointer %p to block of size %u\n", ptr, s);
  return ptr + WORD_SIZE;
}

// Free a previously allocated block
void gfree(void * ptr) {
  printf("gfree called on %p\n", ptr);
  if (ptr == (void *)-1) {
    printf("nothing to free\n");
    return;
  }
  struct block * b = ptr - WORD_SIZE;
  unsigned int s = *((unsigned int *) b);
  printf("gfree: %u\n", s);
  b->size = s;
  if (p == 0) {
    p = b;
  } else {
    while ( ! (p->next <= p && b < p->next) && ! (b < p->next && b > p ) && ! (p->next <= p && b > p) ) {
      //printf("loop, p = %p\n", p);
      p = p->next;
    }
    printf("inserting block %p between %p and %p\n", b, p, p->next);
    b->next = p->next;
  }
  p->next = b;
}

// Test entry point
int main (int argc, char * argv[]) {
  void * ptr;
  unsigned int i = 0;
  init();
  for (i = 0 ; i <= 10 ; i++) {
    ptr = gmalloc(i);
  }
  gfree(ptr);
  return 0;
}
