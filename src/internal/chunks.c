#include "ft_malloc.h"
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>

// int loops;
//
inline static size_t NewFreeChunkMinSize(const enum HEAP_TYPE heap_type) {
  static const size_t sizes[LARGE + 1] = {[TINY] = T_FREE_CHUNK_SIZE, [SMALL] = SMALL_MIN_PAYLOAD, [LARGE] = LARGE_MIN_PAYLOAD};
  return sizes[heap_type];
}

static inline void setFirstFreeChunk(t_heap *heap, t_free_chunk *new_free_chunk) {
  heap->first_free_chunk = new_free_chunk;
  // printStr("1:");
  // printAddr(new_free_chunk, true);
  // printStr("2:");
  // printAddr(heap->first_free_chunk, true);
}

static inline void setFirstChunk(t_heap *g_global, t_chunk *curr_chunk) { g_global->first_chunk = (t_chunk *)curr_chunk; }

inline static void addNewFreeChunk(const size_t new_payload_size, t_free_chunk *unfreeing, t_heap *heap, t_free_chunk *prev_free, t_free_chunk *next_free, const size_t extra_bytes) {
  // new free chunk
  t_free_chunk *new_free_chunk = (void *)(unfreeing) + T_CHUNK_SIZE + new_payload_size;
  new_free_chunk->next = unfreeing->next;
  new_free_chunk->prev = (void *)unfreeing;
  new_free_chunk->payload_bytes = extra_bytes - T_CHUNK_SIZE;
  if (new_free_chunk->payload_bytes > 24480)
    debugVal("", "NOW COUNT", new_free_chunk->payload_bytes);
  // if (loops >= 0) {
  //   debugVal("", "new_free_chunk->payload_bytes", new_free_chunk->payload_bytes);
  //   debugAddr("new_free_chunk addr: ", new_free_chunk);
  // }
  new_free_chunk->is_free = true;
  new_free_chunk->next_free = next_free;
  new_free_chunk->prev_free = prev_free;
  new_free_chunk->next_free = next_free;
  new_free_chunk->heap = heap;
  // curr chunk
  unfreeing->payload_bytes -= extra_bytes;
  unfreeing->next = (void *)new_free_chunk;

  // relink  next_free chunk
  if (next_free)
    next_free->prev_free = new_free_chunk;
  // relink  prev_free chunk
  if (!heap->first_free_chunk || heap->first_free_chunk == unfreeing) {
    setFirstFreeChunk(heap, new_free_chunk);
  } else {
    prev_free->next_free = new_free_chunk;
  }
  debugAddr("first_free_chunk addr: ", heap->first_free_chunk);
}

// TODO check for heap metadata corruption
static t_chunk *unfreeChunk(const size_t bytesNeeded, t_free_chunk *unfreeing, t_heap *heap, const enum HEAP_TYPE heap_type) {
  size_t new_payload_size = bytesNeeded < 16 ? 16 + bytesNeeded : bytesNeeded;
  // printStr("unfreeing->payload_bytes: ");
  // ft_putsize_t(unfreeing->payload_bytes, 1);
  // printStr("\n");
  const size_t extra_bytes = unfreeing->payload_bytes - new_payload_size;
  const bool split_chunk = extra_bytes >= NewFreeChunkMinSize(heap_type);
  t_heap *first_heap = getHeapStart(heap_type);
  // if true we need to create new free chunk from the memory space that is left
  // otherwise the chunk will be bigger than what has been requested
  unfreeing->is_free = false;
  unfreeing->heap = heap;
  t_free_chunk *prev_free = unfreeing->prev_free;
  t_free_chunk *next_free = unfreeing->next_free;
  if (split_chunk) {
    // if (loops >= 0) {
    //   debugVal("", "unfreeing->payload_bytes", unfreeing->payload_bytes);
    //   debugVal("", "new_payload_size", new_payload_size);
    //   debugVal("", "extra_bytes", extra_bytes);
    // }
    addNewFreeChunk(new_payload_size, unfreeing, heap, prev_free, next_free, extra_bytes);
  } else {
    if (!prev_free) {
      setFirstFreeChunk(heap, next_free);
    } else {
      prev_free->next_free = next_free;
      if (next_free)
        next_free->prev_free = prev_free;
    }
  }
  if (!unfreeing->prev || heap->first_chunk == NULL)
    setFirstChunk(first_heap, (t_chunk *)unfreeing);
  // printStr("UNFREED: "); // TODO logs?
  // printAddr(curr_chunk, true);
  return (t_chunk *)unfreeing;
}

inline static t_chunk *findChunkInExistingHeaps(const enum HEAP_TYPE heap_type, t_heap *heap, const size_t bytes_needed) {
  // iterate through heaps
  while (heap) {
    // iterate through heaps
    t_free_chunk *free_chunk = heap->first_free_chunk;
    while (free_chunk) {
      if (free_chunk->payload_bytes >= bytes_needed) {
        t_chunk *new_chunk = unfreeChunk(bytes_needed, free_chunk, heap, heap_type);
        if (new_chunk)
          heap->active_chunk_count++;
        debugInfo("Returning allocated chunk");
        return new_chunk;
      }
      free_chunk = free_chunk->next_free;
    }
    heap = heap->next;
  }
  return NULL;
}

// retrieve last heap from newHeap returned val and trim this up
t_chunk *allocChunk(const size_t bytes_needed) {
  const enum HEAP_TYPE heap_type = getHeapType(bytes_needed);
  t_heap **first_heap = getFirstHeap(heap_type);
  // if (loops >= 0 && first_heap && *first_heap) {
  //   debugAddr("(*first_heap)->first_free_chunk", (*first_heap)->first_free_chunk);
  //   if ((*first_heap)->first_free_chunk)
  //     debugVal("", "(*first_heap)->first_free_chunk->payload_bytes", (*first_heap)->first_free_chunk->payload_bytes);
  // }
  // if there is no first heap create a new heap and check it worked
  if (!first_heap || !*first_heap) {
    if (!newHeap(bytes_needed, heap_type))
      return debugError("Failed to create new heap\n"), NULL;
    first_heap = getFirstHeap(heap_type);
    // if (loops >= 4)
    //   printVal((*first_heap)->first_free_chunk->payload_bytes, "(*first_heap)->first_free_chunk->payload_bytes");
    if (!first_heap || !*first_heap)
      return debugError("get first heap returned null when newHeap succeeded"), NULL;
  }
  t_chunk *new_chunk = findChunkInExistingHeaps(heap_type, *first_heap, bytes_needed);
  if (!new_chunk) {
    if (!newHeap(bytes_needed, heap_type))
      return NULL;
    new_chunk = findChunkInExistingHeaps(heap_type, *first_heap, bytes_needed);
    if (!new_chunk)
      return debugError("Failed to allocatate chunk"), NULL;
  }
  return new_chunk;
}
