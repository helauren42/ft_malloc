#include "ft_malloc.h"
#include <stddef.h>
#include <unistd.h>

/*
 * Returns the min size of the payload of the new allocated free block to create
 * a new free block out of it, it includes the header size in the returned value
 */
inline static size_t NewFreeBlockMinSize(const enum HEAP_TYPE heap_type) {
  static const size_t sizes[LARGE + 1] = {[TINY] = T_FREE_BLOCK_SIZE,
                                          [SMALL] = SMALL_MIN_BYTES,
                                          [LARGE] = LARGE_MIN_BYTES};
  return sizes[heap_type];
}

// TODO check for heap metadata corruption
static t_block *unfreeBlock(const size_t bytesNeeded, t_free_block *curr_block,
                            t_zone *zone, const enum HEAP_TYPE heap_type) {
  const size_t extra_bytes = curr_block->payload_bytes - bytesNeeded;
  const bool split_block = extra_bytes >= NewFreeBlockMinSize(heap_type);
  // if true we need to create new free block from the memory space that is left
  // otherwise the block will be bigger than what has been requested
  curr_block->is_free = false;
  t_free_block *prev_free = curr_block->prev_free;
  t_free_block *next_free = curr_block->next_free;
  if (split_block) {
    // new free block
    t_free_block *new_free_block =
        (void *)(curr_block) + curr_block->payload_bytes + T_BLOCK_SIZE;
    new_free_block->next = curr_block->next;
    new_free_block->prev = curr_block->prev;
    new_free_block->payload_bytes = extra_bytes - T_BLOCK_SIZE;
    new_free_block->is_free = true, new_free_block->next_free = next_free;
    new_free_block->prev_free = prev_free;
    if (!prev_free)
      zone->first_free_block = new_free_block;
    else
      prev_free->next_free = new_free_block;
    // curr block
    curr_block->payload_bytes -= extra_bytes;
  } else {
    if (!prev_free)
      zone->first_free_block = next_free;
    else
      prev_free->next_free = next_free;
  }
  return (t_block *)curr_block;
}

// TODO incr zone block count on successfull call
t_block *getBlock(const size_t bytesNeeded) {
  const enum HEAP_TYPE heap_type = getHeapType(bytesNeeded);
  t_zone *firstZone = getFirstZone(heap_type);
  // if there is no first zone create a new zone and check it worked
  if ((!firstZone && !newZone(bytesNeeded)) ||
      (firstZone = getFirstZone(heap_type)))
    return NULL;
  // iterate through zones
  t_zone *zone = firstZone;
  while (zone) {
    // iterate through blocks
    t_free_block *free_block = zone->first_free_block;
    while (free_block) {
      if (free_block->payload_bytes >= bytesNeeded) {
        t_block *block = unfreeBlock(bytesNeeded, free_block, zone, heap_type);
        if (block)
          zone->block_count++;
        return block;
      }
      free_block = free_block->next_free;
    }
    zone = zone->next;
  }
  return NULL;
}
