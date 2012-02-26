#include <stdio.h>
#include "gmem.h"

#define HEAP_SIZE 16000000
#define WORD_SIZE sizeof(unsigned int)
#define MIN_BLOCK sizeof(struct block)

/* Global memory */
static void * mem[HEAP_SIZE];

/* Memory blocks */
struct block * p; // empty

/* Initialize GMem */
void init() {
  p = (struct block *) mem;
  p->size = HEAP_SIZE;
  p->next = p;
}

/* Max function */
unsigned int max(unsigned int a, unsigned int b) {
  if (a > b) return a;
  return b;
}

/* Allocate a block of bytes */
void * gmalloc(unsigned int size) {
  if (size < 0 || size > HEAP_SIZE || p == 0) {
    return (void *) -1;
  }
  void * ptr = p;
  unsigned int s = max(MIN_BLOCK, size + (size % WORD_SIZE) + WORD_SIZE);
  while (p->next->size != s &&  p->next->size < s + sizeof(struct block)) {
    if (p->next + p->next->size == p->next->next) {
      p->next->size += p->next->next->size;
      p->next->next = p->next->next->next;
    } else {
      p = p->next;
      if (p == ptr) {
        return (void *) -1;
      }
    }
  }
  ptr = p->next;
  if (p->next->size == s) {
    if (p->next == p->next->next) {
      p = 0;
    } else {
      p->next = p->next->next;
    }
  } else {
    p->next = ptr + s;
    p = p->next;
    p->next = (struct block *) ((struct block *) ptr)->next;
    p->size = (unsigned int) ((struct block *) ptr)->size - s;
  }
  *((unsigned int *) ptr) = s;
  return ptr + WORD_SIZE;
}

/* Free a previously allocated block */
void gfree(void * ptr) {
  if (ptr == (void *)-1) {
    return;
  }
  struct block * b = ptr - WORD_SIZE;
  unsigned int s = *((unsigned int *) b);
  b->size = s;
  if (p == 0) {
    p = b;
  } else {
    while ( ! (p->next <= p && b < p->next) && ! (b < p->next && b > p ) && ! (p->next <= p && b > p) ) {
      p = p->next;
    }
    b->next = p->next;
  }
  p->next = b;
}

/* Test entry point 
int main (int argc, char * argv[]) {
  void * ptr;
  unsigned int i = 0;
  init();
  for (i = 0 ; i <= 10 ; i++) {
    ptr = gmalloc(i);
  }
  gfree(ptr);
  return 0;
}*/
