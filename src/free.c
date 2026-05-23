#include "ft_malloc.h"
#include <sys/mman.h>
#include <unistd.h>

inline static t_free_chunk *defragChunks(t_free_chunk *new_free) {
  t_chunk *next = new_free->next;
  t_chunk *prev = new_free->prev;
  if (next && next->is_free) {
    new_free->next = next->next;
    new_free->payload_bytes += next->payload_bytes + T_CHUNK_SIZE;
  }
  if (prev && prev->is_free) {
    t_chunk *before = prev->prev;
    new_free->prev = before;
    new_free->payload_bytes += prev->payload_bytes + T_CHUNK_SIZE;
    ft_memcpy(prev, new_free, T_FREE_CHUNK_SIZE);
    return (t_free_chunk *)prev;
  }
  return new_free;
}

inline static void prependFreeChunk(t_free_chunk *new_free_chunk,
                                    t_heap *heap) {
  new_free_chunk->next_free = heap->first_free_chunk;
  if (!heap->first_free_chunk)
    heap->first_free_chunk = new_free_chunk;
  else {
    new_free_chunk->next_free = heap->first_free_chunk;
    heap->first_free_chunk = new_free_chunk;
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
  t_free_chunk *new_free = defragChunks((t_free_chunk *)chunk);
  new_free->is_free = true;
  t_heap *heap = new_free->heap;
  heap->active_chunk_count--;
  if (heap->active_chunk_count == 0)
    removeHeap(heap);
  else {
    prependFreeChunk(new_free, heap);
  }
}
