#include "ft_malloc.h"
#include <stdbool.h>
#include <stdint.h>
#include <sys/mman.h>
#include <unistd.h>

inline static bool mergeNext(t_free_chunk *new_free) {
  t_free_chunk *next = (t_free_chunk *)new_free->next;
  if (!next || !next->is_free)
    return false;
  new_free->payload_bytes += next->payload_bytes + T_CHUNK_SIZE;
  new_free->next = next->next;
  new_free->next_free = next->next_free;
  if (next->next_free)
    next->next_free->prev_free = new_free;
  return true;
}

inline static t_free_chunk *mergePrev(t_free_chunk *new_free) {
  t_free_chunk *prev_free = (t_free_chunk *)new_free->prev;
  t_free_chunk *next_free = new_free->next_free;

  if (!prev_free || !prev_free->is_free)
    return new_free;

  prev_free->payload_bytes += new_free->payload_bytes + T_CHUNK_SIZE;
  prev_free->next = new_free->next;
  new_free->next->prev = new_free->prev;
  next_free->prev_free = prev_free;

  return prev_free;
}

inline static void unlinkFreeChunk(t_free_chunk *chunk) {
  t_free_chunk *before = chunk->prev_free;
  t_free_chunk *after = chunk->next_free;
  if (before)
    before->next_free = after;
  else
    chunk->heap->first_free_chunk = after;
  if (after)
    after->prev_free = before;
}

inline static void prependFreeChunk(t_free_chunk *new_free_chunk,
                                    t_heap *heap) {
  new_free_chunk->prev_free = NULL;
  new_free_chunk->next_free = heap->first_free_chunk;

  if (heap->first_free_chunk) {
    heap->first_free_chunk->prev_free = new_free_chunk;
  }

  heap->first_free_chunk = new_free_chunk;
}

void ft_free(void *ptr) {
  if (!ptr)
    return;

  g_global.function_called = FREE;
  t_chunk *chunk = getHeaderAddr(ptr);
  if (chunk->is_free) {
    errorDoubleFree();
    return;
  }
  t_heap *heap = chunk->heap;
  heap->active_chunk_count--;
  if (heap->active_chunk_count == 0 && (heap->next || heap->prev)) {
    removeHeap(heap);
    return;
  }

  chunk->is_free = true;
  t_free_chunk *new_free = (t_free_chunk *)chunk;
  t_free_chunk *merged = mergePrev(new_free);
  mergeNext(merged);
  // unlinkFreeChunk(merged);
  // prependFreeChunk(merged, heap);
}
