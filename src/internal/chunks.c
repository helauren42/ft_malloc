#include "ft_malloc.h"
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>

inline static size_t NewFreeChunkMinSize(const enum HEAP_TYPE heap_type) {
  static const size_t sizes[LARGE + 1] = {[TINY] = T_FREE_CHUNK_SIZE,
                                          [SMALL] = SMALL_MIN_PAYLOAD,
                                          [LARGE] = LARGE_MIN_PAYLOAD};
  return sizes[heap_type];
}

static inline void setFirstFreeChunk(t_heap *g_global, t_heap *heap,
                                     t_free_chunk *new_free_chunk) {
  heap->first_free_chunk = new_free_chunk;
  g_global->first_free_chunk = new_free_chunk;
}

static inline void setFirstChunk(t_heap *g_global, t_chunk *curr_chunk) {
  g_global->first_chunk = (t_chunk *)curr_chunk;
}

inline static void createNewFreeChunk(const size_t bytesNeeded,
                                      t_free_chunk *curr_chunk, t_heap *heap,
                                      t_free_chunk *prev_free,
                                      t_free_chunk *next_free,
                                      const size_t extra_bytes,
                                      t_heap *g_global) {
  // new free chunk
  t_free_chunk *new_free_chunk =
      (void *)(curr_chunk) + T_CHUNK_SIZE + bytesNeeded;
  new_free_chunk->next = curr_chunk->next;
  new_free_chunk->prev = (void *)curr_chunk;
  new_free_chunk->payload_bytes = extra_bytes - T_CHUNK_SIZE;
  new_free_chunk->is_free = true;
  new_free_chunk->next_free = next_free;
  new_free_chunk->prev_free = prev_free;
  new_free_chunk->next_free = next_free;
  new_free_chunk->heap = heap;

  if (!prev_free) {
    setFirstFreeChunk(g_global, heap, new_free_chunk);
  } else
    prev_free->next_free = new_free_chunk;
  if (!curr_chunk->prev)
    setFirstChunk(g_global, (t_chunk *)curr_chunk);
  // curr chunk
  curr_chunk->payload_bytes -= extra_bytes;
  curr_chunk->next = (void *)new_free_chunk;
}

// TODO check for heap metadata corruption
static t_chunk *unfreeChunk(const size_t bytesNeeded, t_free_chunk *curr_chunk,
                            t_heap *heap, const enum HEAP_TYPE heap_type) {
  const size_t extra_bytes = curr_chunk->payload_bytes - bytesNeeded;
  const bool split_chunk = extra_bytes >= NewFreeChunkMinSize(heap_type);
  t_heap *g_global = getHeapStart(heap_type);
  // if true we need to create new free chunk from the memory space that is left
  // otherwise the chunk will be bigger than what has been requested
  curr_chunk->is_free = false;
  curr_chunk->heap = heap;
  t_free_chunk *prev_free = curr_chunk->prev_free;
  t_free_chunk *next_free = curr_chunk->next_free;
  if (split_chunk) {
    createNewFreeChunk(bytesNeeded, curr_chunk, heap, prev_free, next_free,
                       extra_bytes, g_global);
  } else {
    if (!prev_free) {
      setFirstFreeChunk(g_global, heap, next_free);
    } else {
      prev_free->next_free = next_free;
      next_free->prev_free = prev_free;
    }
    if (!curr_chunk->prev)
      setFirstChunk(g_global, (t_chunk *)curr_chunk);
  }
  // printStr("UNFREED: "); // TODO logs?
  // printAddr(curr_chunk, true);
  return (t_chunk *)curr_chunk;
}

t_chunk *allocChunk(const size_t bytesNeeded) {
  const enum HEAP_TYPE heap_type = getHeapType(bytesNeeded);
  t_heap **firstHeap = getFirstHeap(heap_type);
  // if there is no first heap create a new heap and check it worked
  if (!firstHeap || !*firstHeap) {
    if (!newHeap(bytesNeeded))
      return debugError("Failed to create new heap\n"), NULL;
    firstHeap = getFirstHeap(heap_type);
    if (!firstHeap || !*firstHeap)
      return debugError("get first heap returned null when newHeap succeeded"),
             NULL;
  }
  // iterate through heaps
  t_heap *heap = *firstHeap;
  while (heap) {
    // iterate through heaps
    t_free_chunk *free_chunk = heap->first_free_chunk;
    while (free_chunk) {
      if (free_chunk->payload_bytes >= bytesNeeded) {
        t_chunk *chunk = unfreeChunk(bytesNeeded, free_chunk, heap, heap_type);
        if (chunk)
          heap->active_chunk_count++;
        debugInfo("Returning allocated chunk");
        return chunk;
      }
      free_chunk = free_chunk->next_free;
    }
    heap = heap->next;
    printStr("Heap addr: ");
    printAddr(heap, true);
  }
  debugError("Failed to allocatate chunk");
  return NULL;
}
