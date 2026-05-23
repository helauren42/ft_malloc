#include "ft_malloc.h"
#include <sys/mman.h>

inline static t_free_chunk *freeAndMergeChunks(t_free_chunk *free_chunk) {
  free_chunk->is_free = true;
  t_chunk *next = free_chunk->next;
  t_chunk *prev = free_chunk->prev;
  if (next && next->is_free) {
    free_chunk->payload_bytes += next->payload_bytes + T_CHUNK_SIZE;
  }
  if (prev && prev->is_free) {
    free_chunk->payload_bytes += prev->payload_bytes + T_CHUNK_SIZE;
    t_chunk *before = prev->prev;
    ft_memcpy(prev, free_chunk, T_FREE_CHUNK_SIZE);
    if (before) {
      before->next = prev;
      prev->prev = before;
    }
    return (t_free_chunk *)prev;
  }
  return free_chunk;
}

inline static void appendFreeChunk(t_free_chunk *new_free_chunk, t_heap *heap) {
  if (!heap->first_free_chunk)
    heap->first_free_chunk = new_free_chunk;
  else {
    t_free_chunk *free_chunk = heap->first_free_chunk;
    while (free_chunk->next_free)
      free_chunk = free_chunk->next_free;
    free_chunk->next_free = new_free_chunk;
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
  t_free_chunk *free_chunk = freeAndMergeChunks((t_free_chunk *)chunk);
  t_heap *heap = free_chunk->heap;
  heap->active_chunk_count--;
  if (heap->active_chunk_count == 0)
    removeHeap(heap);
  else
    appendFreeChunk(free_chunk, heap);
}
