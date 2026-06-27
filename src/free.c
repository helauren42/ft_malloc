#include "ft_malloc.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>

inline static bool mergeNext(t_free_chunk *new_free) {
  t_free_chunk *next = (t_free_chunk *)new_free->next;
  if (!next || !next->is_free)
    return false;
  debugVal("merging with next", "next->payload_bytes", next->payload_bytes);
  new_free->payload_bytes += next->payload_bytes + T_CHUNK_SIZE;
  new_free->next = next->next;
  new_free->next_free = next->next_free;
  if (next->next_free)
    next->next_free->prev_free = new_free;
  if (!next->prev_free) {
    new_free->heap->first_free_chunk = new_free;
  }
  return true;
}

inline static t_free_chunk *mergePrev(t_free_chunk *new_free) {
  t_free_chunk *prev_free = (t_free_chunk *)new_free->prev;
  if (!prev_free || !prev_free->is_free)
    return new_free;
  debugInfo("merging with prev");
  prev_free->payload_bytes = prev_free->payload_bytes + new_free->payload_bytes + T_CHUNK_SIZE;
  if (!prev_free->prev_free)
    new_free->heap->first_free_chunk = prev_free;
  // relink prev_free to next in all chunks list
  prev_free->next = new_free->next;
  return (t_free_chunk *)prev_free;
}

inline void prependFreeChunk(t_free_chunk *new_free_chunk, t_heap *heap) {
  if (!heap->first_free_chunk) {
    heap->first_free_chunk = new_free_chunk;
    new_free_chunk->prev_free = NULL;
    new_free_chunk->next_free = NULL;
  } else {
    new_free_chunk->next_free = heap->first_free_chunk;
    new_free_chunk->prev_free = NULL;
    heap->first_free_chunk->prev_free = new_free_chunk;
    heap->first_free_chunk = new_free_chunk;
  }
}

void free(void *ptr) {
  if (!ptr)
    return;
  g_global.function_called = FREE;
  t_chunk *chunk = getHeaderAddr(ptr);
  // printStr("Freeing this addr:"); // TODO logs?
  // printAddr(chunk, true);
  if (chunk->is_free) {
    errorDoubleFree();
    return;
  }
  t_heap *heap = chunk->heap;
  debugAddr("start first_free_chunk addr: ", heap->first_free_chunk);
  heap->active_chunk_count--;
  if (heap->active_chunk_count == 0 && (heap->next || heap->prev)) {
    removeHeap(heap);
    return;
  }
  chunk->is_free = true;
  t_free_chunk *new_free = mergePrev((t_free_chunk *)chunk);
  debugVal("1", "new_free->payload_bytes: ", new_free->payload_bytes);
  // if merged than there is no need to preprend
  const bool mergedNext = mergeNext(new_free);
  debugVal("2", "new_free->payload_bytes: ", new_free->payload_bytes);
  if ((uintptr_t)new_free != (uintptr_t)chunk || mergedNext) {
    debugAddr("end first_free_chunk addr: ", heap->first_free_chunk);
    return;
  }
  // the prev and next free chunk pointers are in the payload and will be overwritten so we can't use those to reinsert the chunk
  prependFreeChunk(new_free, heap);
  printFreeChunks(heap);
  debugAddr("end first_free_chunk addr", heap->first_free_chunk);
}
