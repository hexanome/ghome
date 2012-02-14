#ifndef __GMEM_H
#define __GMEM_H

void * gmalloc(unsigned int size);
void gfree(void * ptr);

struct block {
  unsigned int size;
  struct block * next;
};

#endif
