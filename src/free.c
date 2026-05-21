#include "ft_malloc.h"
#include <sys/mman.h>

inline static void mergeBlocks(t_block *block) {
  t_block *next = block->next;
  t_block *prev = block->prev;
  if (next && next->is_free) {
    block->payload_bytes += next->payload_bytes + T_BLOCK_SIZE;
  }
  if (prev && prev->is_free) {
    block->payload_bytes += prev->payload_bytes + T_BLOCK_SIZE;
  }
}

void free(void *ptr) {
  if (!ptr)
    return;
  g_global.function_called = FREE;
  t_block *block = getHeaderAddr(ptr);
  // printStr("Freeding this addr:"); // TODO logs?
  // printAddr(block, true);
  if (block->is_free) {
    errorDoubleFree();
    return;
  }
  block->is_free = true;
  mergeBlocks(block);
  t_zone *zone = block->zone;
  zone->active_block_count--;
  if (zone->active_block_count == 0)
    removeZone(block, zone);
}
