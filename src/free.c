#include "ft_malloc.h"
#include <stdio.h>
#include <sys/mman.h>

inline static void mergeBlocks(t_block *block, t_block *next) {
  if (next && next->is_free) {
    block->payload_bytes += next->payload_bytes + T_BLOCK_SIZE;
  }
}

void free(void *ptr) {
  if (!ptr)
    return;
  debugInfo("Called free");
  g_heaps.function_called = FREE;
  t_block *block = getHeaderAddr(ptr);
  printStr("Freeding this addr:");
  printAddr(block, true);
  block->is_free = true;
  t_block *next = block->next;
  mergeBlocks(block, next);
  t_zone *zone = block->zone;
  zone->active_block_count--;
  if (zone->active_block_count == 0)
    munmap(zone, 1);
}
