#include "gmem.h"

// Global memory
static void * mem[65535];

// Free block
struct block * p = (struct block *) mem;

void init() {
  p->size = sizeof mem;
  p->next = p;
}

// Allocate p->next or following
void * gmalloc(unsigned int size) {
  void * ret = -1;
  block *temp;
  if ( size < p->next->size ) {
    ret = p->next;
    temp = (struct block *)(p->next + size);
    temp->size = p->next->size - size;
    temp->next = p->next;
    p->next = temp;
  }
  return ret;
}

void gfree(void * ptr) {
}

int main (int argc, char * argv[]) {
  return 0;
}
