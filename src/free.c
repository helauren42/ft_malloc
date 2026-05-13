#include "ft_malloc.h"
#include <sys/mman.h>

void defragment(t_zone *zone) {
  t_block *block = zone->first_block;
  while (block->next) {
    if (block->freed && block->next->freed) {
      // merge blocks
      block->bytes += block->next->bytes;
      // TODO remove next block somehow
      block->next += block->bytes;
      if (SECURE) {
      }
    }
  }
}

void ft_free(void *ptr) {
  if (!ptr)
    return;
  printf("Freeing '%p'\n", ptr);
  munmap(ptr, 1);
}
