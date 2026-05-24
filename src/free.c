#include "ft_malloc.h"
#include <sys/mman.h>
#include <unistd.h>

inline static void mergeNext(t_free_chunk *new_free) {
  t_free_chunk *next_free = (t_free_chunk *)new_free->next;
  if (!next_free || !next_free->is_free)
    return;
  new_free->payload_bytes += next_free->payload_bytes + T_CHUNK_SIZE;
  new_free->next = next_free->next;
  new_free->next_free = next_free->next_free;
  if (next_free->next_free)
    next_free->next_free->prev_free = new_free;
}

inline static t_free_chunk *mergePrev(t_free_chunk *new_free) {
  t_free_chunk *prev_free = (t_free_chunk *)new_free->prev;
  t_free_chunk *next_free = (t_free_chunk *)new_free->next_free;
  if (!prev_free || !prev_free->is_free)
    return new_free;
  prev_free->payload_bytes += new_free->payload_bytes + T_CHUNK_SIZE;
  // relink before prev to next_free in free list or the heap's first_free_chunk
  t_free_chunk *before_prev_free = prev_free->prev_free;
  if (before_prev_free) {
    before_prev_free->next_free = next_free;
  } else {
    new_free->heap->first_free_chunk = prev_free;
  }
  // no need to relink before prev in all chunks list because it already points to prev
  // relink prev_free to next in all chunks list
  prev_free->next = new_free->next;
  return (t_free_chunk *)prev_free;
}

// inline static void prependFreeChunk(t_free_chunk *new_free_chunk,
//                                     t_heap *heap) {
//   new_free_chunk->next_free = heap->first_free_chunk;
//   if (!heap->first_free_chunk)
//     heap->first_free_chunk = new_free_chunk;
//   else {
//     new_free_chunk->next_free = heap->first_free_chunk;
//     heap->first_free_chunk = new_free_chunk;
//   }
// }

void ft_free(void *ptr) {
  if (!ptr)
    return;
  printLine("CALLED FREE");
  g_global.function_called = FREE;
  t_chunk *chunk = getHeaderAddr(ptr);
  // printStr("Freeding this addr:"); // TODO logs?
  // printAddr(chunk, true);
  if (chunk->is_free) {
    errorDoubleFree();
    return;
  }
  t_heap *heap = chunk->heap;
  heap->active_chunk_count--;
  if (heap->active_chunk_count == 0) {
    removeHeap(heap);
    return;
  }
  // if heap still active then free chunk and try to defragment
  chunk->is_free = true;
  t_free_chunk *new_free = mergePrev((t_free_chunk *)chunk);
  new_free->is_free = true;
  mergeNext(new_free);
  // else {
  //   prependFreeChunk(new_free, heap);
  // }
  printLine("End of free");
  printFreeChunks(heap);
}
