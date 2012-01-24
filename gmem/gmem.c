#include "gmem.h"

// Global memory
static void * mem[65535];

// Memory blocks
struct block * p; // empty
struct block * u; // used

// Initialize GMem
void init() {
  p = (struct block *) mem;
  p->size = sizeof(mem);
  p->next = p;
  u = (struct block *) -1;
}

// Allocate a block of bytes
void * gmalloc(unsigned int size) {
  struct block * ptr = p;
  while (p->next->size < size) {
    // TODO try merge
    p = p->next;
    if (p == ptr) return (void *) -1;
  }
  ptr = p->next;
  p->next = ptr + size;
  p->next->size = ptr->size - size;
  p->next->next = ptr->next;
  return ptr;
}

// Free a previously allocated block
void gfree(void * ptr) {
  struct block * d = u;
  do {
    if (d == ptr) {
      // TODO free block
      return;
    }
    d = u->next;
  } while (d != u);
}

int main (int argc, char * argv[]) {
  return 0;
}
