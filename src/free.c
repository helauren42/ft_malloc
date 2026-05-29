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
  if (next->next)
    next->next->prev = (t_chunk *)new_free;
  return true;
}

inline static t_free_chunk *mergePrev(t_free_chunk *new_free) {
  t_free_chunk *prev_free = (t_free_chunk *)new_free->prev;
  if (!prev_free || !prev_free->is_free)
    return new_free;
  t_chunk *next = new_free->next;
  prev_free->payload_bytes += new_free->payload_bytes + T_CHUNK_SIZE;
  prev_free->next = next;
  prev_free->next = new_free->next;
  return (t_free_chunk *)prev_free;
}

inline static void prependFreeChunk(t_free_chunk *new_free_chunk,
                                    t_heap *heap) {
  if (!heap->first_free_chunk) {
    heap->first_free_chunk = new_free_chunk;
    new_free_chunk->prev_free = NULL;
    new_free_chunk->next_free = NULL;
  } else {
    new_free_chunk->next_free = heap->first_free_chunk;
    heap->first_free_chunk = new_free_chunk;
  }
}

void ft_free(void *ptr) {
  if (!ptr)
    return;
  g_global.function_called = FREE;
  t_chunk *new_free = getHeaderAddr(ptr);
  // printStr("Freeding this addr:"); // TODO logs?
  // printAddr(chunk, true);
  if (new_free->is_free) {
    errorDoubleFree();
    return;
  }
  t_heap *heap = new_free->heap;
  heap->active_chunk_count--;
  if (heap->active_chunk_count == 0 && (heap->next || heap->prev)) {
    removeHeap(heap);
    return;
  }
  new_free->is_free = true;
  t_free_chunk *merged = mergePrev((t_free_chunk *)new_free);
  // if merged than there is no need to preprend
  if ((uintptr_t)merged != (uintptr_t)new_free || mergeNext(merged))
    return;
  prependFreeChunk(merged, heap);
  // printFreeChunks(heap);
}
