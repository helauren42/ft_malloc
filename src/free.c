#include "ft_malloc.h"
#include <stdio.h>
#include <sys/mman.h>

void defragment(t_zone *zone) {
  t_block *block = zone->first_block;
  while (block->next) {
    if (block->freed && block->next->freed) {
      // merge blocks
      block->payload_bytes += block->next->payload_bytes;
      // TODO remove next block somehow
      block->next += block->payload_bytes;
      if (SECURE) {
      }
    }
  }
}

void free(void *ptr) {
  if (!ptr)
    return;
  global.function_called = MALLOC;
  printf("Freeing '%p'\n", ptr);
  munmap(ptr, 1);
}
