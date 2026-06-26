#include "ft_malloc.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <unistd.h>

static inline void cpy_header(const uint8_t *src, uint8_t *dest) {
  for (size_t i = 0; i < T_FREE_CHUNK_SIZE; i++) {
    dest[i] = src[i];
  }
}

static inline void cpy_payload(const uint8_t *old, uint8_t *new, const size_t payload_bytes) {
  for (size_t i = 0; i < payload_bytes; i++) {
    new[i] = old[i];
  }
}

static inline bool try_expand(t_chunk *chunk, t_free_chunk *next, const size_t diff) {
  if (!chunk->next || !chunk->next->is_free)
    return false;
  static const size_t min_retractable_payload = T_CHUNK_SIZE - T_FREE_CHUNK_SIZE;
  const size_t new_next_payload_size = next->payload_bytes - diff;
  if (new_next_payload_size < min_retractable_payload)
    return false;
  static uint8_t buffer[T_FREE_CHUNK_SIZE];
  cpy_header((uint8_t *)next, buffer); // prev next header stored in buffer on stack to not corrupt heap

  t_free_chunk *new_next = (t_free_chunk *)((uintptr_t)next + diff);
  cpy_header(buffer, (uint8_t *)new_next);
  chunk->next = (t_chunk *)new_next;
  chunk->payload_bytes += diff;
  new_next->payload_bytes = new_next_payload_size;
  debugInfo("try expand on realloc");
  return true;
}

static inline void retract(t_chunk *chunk, t_free_chunk *next, const size_t diff) {
  static const size_t min_payload_size = T_FREE_CHUNK_SIZE - T_CHUNK_SIZE;
  static uint8_t buffer[T_FREE_CHUNK_SIZE];
  if (next && next->is_free) {
    size_t new_chunk_payload_size = chunk->payload_bytes - diff;
    if (new_chunk_payload_size <= min_payload_size)
      new_chunk_payload_size = min_payload_size;
    t_free_chunk *new_next = (t_free_chunk *)((uintptr_t)next - diff);
    cpy_header((uint8_t *)next, buffer);
    cpy_header(buffer, (uint8_t *)new_next);
    new_next->payload_bytes += diff;
    chunk->next = (t_chunk *)new_next;
    chunk->payload_bytes -= diff;
  } else if (NewFreeChunkMinSize(chunk->heap->heap_type) <= diff) {
    chunk->payload_bytes -= diff;
    t_free_chunk *new_free_chunk = (t_free_chunk *)((uintptr_t)chunk + T_CHUNK_SIZE + chunk->payload_bytes);
    t_heap *heap = chunk->heap;
    new_free_chunk->prev = chunk;
    new_free_chunk->next = NULL;
    new_free_chunk->next_free = NULL;
    new_free_chunk->prev_free = NULL;
    new_free_chunk->heap = heap;
    new_free_chunk->payload_bytes = diff - T_CHUNK_SIZE;
    new_free_chunk->is_free = true;
    prependFreeChunk(new_free_chunk, heap);
    chunk->next = (t_chunk *)new_free_chunk;
  }
}

void *ft_realloc(void *ptr, size_t size) {
  t_chunk *chunk = getHeaderAddr(ptr);
  const size_t curr_size = chunk->payload_bytes;
  bool toFree = false;
  if (!ptr)
    return ft_malloc(size);
  if (size == curr_size)
    return ptr;
  void *ret;
  if (size == 0) {
    toFree = true;
    ret = NULL;
  } else if (size > curr_size) {
    if (try_expand(chunk, (t_free_chunk *)chunk->next, size - curr_size))
      ret = ptr;
    else {
      debugInfo("new malloc call on realloc");
      void *new_ptr = ft_malloc(size);
      if (!new_ptr)
        ret = NULL;
      t_chunk *new_chunk = getHeaderAddr(new_ptr);
      const size_t size_copied = new_chunk->payload_bytes < chunk->payload_bytes ? new_chunk->payload_bytes : chunk->payload_bytes;
      ft_memcpy(new_ptr, ptr, size_copied);
      toFree = true;
      ret = new_ptr;
    }
  } else {
    retract(chunk, (t_free_chunk *)chunk->next, curr_size - size);
    ret = ptr;
  }
  // copy data from previous address to new address
  if (ret != NULL && ret != ptr) {
    const size_t new_size = getHeaderAddr(ret)->payload_bytes;
    const size_t n = new_size < curr_size ? new_size : curr_size;
    ft_memcpy(ret, ptr, n);
  }
  if (toFree)
    ft_free(ptr);
  return ret;
}
