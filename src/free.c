#include "ft_malloc.h"
#include <sys/mman.h>

inline static void mergeBlocks(t_chunk *chunk) {
  t_chunk *next = chunk->next;
  t_chunk *prev = chunk->prev;
  if (next && next->is_free) {
    chunk->payload_bytes += next->payload_bytes + T_CHUNK_SIZE;
  }
  if (prev && prev->is_free) {
    chunk->payload_bytes += prev->payload_bytes + T_CHUNK_SIZE;
  }
}

void free(void *ptr) {
  if (!ptr)
    return;
  g_global.function_called = FREE;
  t_chunk *chunk = getHeaderAddr(ptr);
  // printStr("Freeding this addr:"); // TODO logs?
  // printAddr(chunk, true);
  if (chunk->is_free) {
    errorDoubleFree();
    return;
  }
  chunk->is_free = true;
  mergeBlocks(chunk);
  t_heap *heap = chunk->heap;
  heap->active_chunk_count--;
  if (heap->active_chunk_count == 0)
    removeZone(chunk, heap);
}
