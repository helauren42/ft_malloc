#include "ft_malloc.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <sys/mman.h>

t_arenas g_global = {NULL, NULL, NULL, 0};

inline t_heap **getFirstHeap(const enum HEAP_TYPE heap_type) {
  switch (heap_type) {
  case TINY:
    return &g_global.tiny_first;
  case SMALL:
    return &g_global.small_first;
  case LARGE:
    return &g_global.large_first;
  default:
    debugError("getFirstHeap default case");
    return NULL;
  }
}

inline static t_heap *newLimitedHeap(const enum HEAP_TYPE heap_type) {
  size_t heap_size = heap_type == TINY ? TINY_HEAP_SIZE : SMALL_HEAP_SIZE;
  t_heap *new_heap = NULL;
  new_heap = mmap(NULL, heap_size, MMAP_PROT, MMAP_FLAGS, -1, 0);
  if (new_heap == MAP_FAILED)
    return MAP_FAILED;
  return new_heap;
}

inline static size_t newHeapSize(const size_t payloadSize,
                                 const enum HEAP_TYPE heap_type) {
  if (heap_type == TINY)
    return TINY_HEAP_SIZE;
  else if (heap_type == SMALL)
    return SMALL_HEAP_SIZE;
  else {
    size_t total = payloadSize + T_HEAP_SIZE + T_CHUNK_SIZE;
    return PAGE_SIZE * (total / PAGE_SIZE + (total % PAGE_SIZE > 0 ? 1 : 0));
  }
}

inline static void initNewHeap(t_heap *new_heap, const enum HEAP_TYPE heap_type,
                               const size_t bytesRequested) {
  // new heap
  new_heap->heap_type = heap_type;
  new_heap->next = NULL;
  new_heap->prev = NULL; // prev is set inside appendNewHeap
  new_heap->first_free_chunk = (void *)new_heap + T_HEAP_SIZE;
  new_heap->first_chunk = NULL;
  new_heap->active_chunk_count = 0;
  new_heap->size = newHeapSize(bytesRequested, heap_type);
  // first_free_chunk
  t_free_chunk *first_free_chunk = new_heap->first_free_chunk;
  first_free_chunk->prev = NULL;
  first_free_chunk->next = NULL;
  first_free_chunk->is_free = true;
  first_free_chunk->payload_bytes = new_heap->size - T_HEAP_SIZE;
}

inline static void appendNewHeap(t_heap *new_heap,
                                 const enum HEAP_TYPE heap_type) {
  t_heap **first_heap;
  switch (heap_type) {
  case TINY:
    first_heap = &g_global.tiny_first;
    break;
  case SMALL:
    first_heap = &g_global.small_first;
    break;
  case LARGE:
    first_heap = &g_global.large_first;
    break;
  default:
    debugError("appendNewHeap heap_type error");
    return;
  }
  if (!*first_heap)
    *first_heap = new_heap;
  else {
    t_heap *next_heap = *first_heap;
    next_heap->prev = new_heap;
    new_heap->next = next_heap;
    *first_heap = new_heap;
    debugInfo("appended new heap");
  }
}

inline t_heap *newHeap(const size_t bytesRequested, const enum HEAP_TYPE heap_type) {
  t_heap *new_heap;
  if (heap_type == LARGE) {
    new_heap = mmap(NULL, bytesRequested + T_HEAP_SIZE + T_CHUNK_SIZE, MMAP_PROT, MMAP_FLAGS, -1, 0);
  } else {
    new_heap = newLimitedHeap(heap_type);
  }
  if (new_heap == MAP_FAILED)
    return debugError("map failed bruhh"), NULL;
  initNewHeap(new_heap, heap_type, bytesRequested);
  appendNewHeap(new_heap, heap_type);
  return new_heap;
}

inline void removeHeap(t_heap *heap) {
  debugInfo("REMOVING HEAP");
  t_heap *prev = heap->prev;
  t_heap *next = heap->next;
  if (prev) {
    prev->next = next;
  } else {
    const enum HEAP_TYPE heap_type = heap->heap_type;
    t_heap **heap = getFirstHeap(heap_type);
    if (next)
      (*heap) = next;
    else
      (*heap) = NULL;
  }
  munmap(heap, heap->size);
}
