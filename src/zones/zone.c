#include "ft_malloc.h"
#include <stdbool.h>
#include <sys/mman.h>

t_global global = {NULL, NULL, NULL};

static inline t_zone *newLimitedZone(const enum HEAP_TYPE heap_type) {
  size_t zone_size = heap_type == TINY ? TINY_ZONE_SIZE : SMALL_ZONE_SIZE;
  t_zone *new_zone = NULL;
  new_zone = mmap(NULL, zone_size, MMAP_PROT, MMAP_FLAGS, -1, 0);
  if (new_zone == MAP_FAILED)
    return MAP_FAILED;
  return new_zone;
}

static inline void initNewZone(t_zone *new_zone,
                               const enum HEAP_TYPE heap_type) {
  // new zone
  new_zone->next = NULL;
  new_zone->block_count = 0;
  new_zone->first_free_block = (void *)(new_zone + 1);
  // first_free_block
  t_free_block *first_free_block = new_zone->first_free_block;
  first_free_block->prev = NULL;
  first_free_block->next = NULL;
  // both
  switch (heap_type) {
  case TINY:
    new_zone->available_bytes = TINY_ZONE_SIZE - T_ZONE_SIZE - T_BLOCK_SIZE;
    first_free_block->bytes = TINY_ZONE_SIZE - T_BLOCK_SIZE;
    break;
  case SMALL:
    new_zone->available_bytes = SMALL_ZONE_SIZE - T_ZONE_SIZE - T_BLOCK_SIZE;
    first_free_block->bytes = TINY_ZONE_SIZE - T_BLOCK_SIZE;
    break;
  case LARGE:
    new_zone->available_bytes = 0; // TODO
    first_free_block->bytes = 0;   // TODO
    break;
  }
}

inline t_zone *newZone(const size_t size) {
  const enum HEAP_TYPE heap_type = getZoneType(size);
  // creating new zone
  t_zone *new_zone;
  if (heap_type == LARGE) {
    new_zone = mmap(NULL, size, MMAP_PROT, MMAP_FLAGS, -1, 0);
  } else {
    new_zone = newLimitedZone(heap_type);
  }
  if (new_zone == MAP_FAILED)
    return NULL;
  // appending new zone to list of zones
  t_zone *first_zone = getFirstZone(heap_type);
  if (first_zone) {
    getLastZone(first_zone)->next = new_zone;
  } else {
    first_zone = new_zone;
  }
  initNewZone(new_zone, heap_type);
  return new_zone;
}
