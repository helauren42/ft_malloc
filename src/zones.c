#include "ft_malloc.h"
#include <stdbool.h>
#include <stdio.h>
#include <sys/mman.h>

t_global global = {NULL, NULL, NULL, NULL, NULL, NULL, 0};

inline static t_zone **getLastZone(const enum HEAP_TYPE heap_type) {
  switch (heap_type) {
  case TINY:
    return &global.tiny_last;
  case SMALL:
    return &global.small_last;
  case LARGE:
    return &global.large_last;
  default:
    debugError("getFirstZone default case");
    return NULL;
  }
}

inline t_zone *getFirstZone(const enum HEAP_TYPE heap_type) {
  switch (heap_type) {
  case TINY:
    return global.tiny_first;
  case SMALL:
    return global.small_first;
  case LARGE:
    return global.large_first;
  default:
    debugError("getFirstZone default case");
    return NULL;
  }
}

inline static t_zone *newLimitedZone(const enum HEAP_TYPE heap_type) {
  size_t zone_size = heap_type == TINY ? TINY_ZONE_SIZE : SMALL_ZONE_SIZE;
  t_zone *new_zone = NULL;
  new_zone = mmap(NULL, zone_size, MMAP_PROT, MMAP_FLAGS, -1, 0);
  if (new_zone == MAP_FAILED)
    return MAP_FAILED;
  return new_zone;
}

inline static void initNewZone(t_zone *new_zone,
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
    new_zone->free_bytes = TINY_ZONE_SIZE - T_ZONE_SIZE - T_BLOCK_SIZE;
    first_free_block->payload_bytes = TINY_ZONE_SIZE - T_BLOCK_SIZE;
    break;
  case SMALL:
    new_zone->free_bytes = SMALL_ZONE_SIZE - T_ZONE_SIZE - T_BLOCK_SIZE;
    first_free_block->payload_bytes = SMALL_ZONE_SIZE - T_BLOCK_SIZE;
    break;
  case LARGE:
    new_zone->free_bytes = 0;            // TODO
    first_free_block->payload_bytes = 0; // TODO
    break;
  default:
    debugError("errorHeapMetadataCorruption default case");
  }
}

inline static void appendNewZone(t_zone *new_zone,
                                 const enum HEAP_TYPE heap_type) {
  t_zone **last_zone;
  t_zone **first_zone;
  switch (heap_type) {
  case TINY:
    last_zone = &global.tiny_last;
    first_zone = &global.tiny_first;
    break;
  case SMALL:
    last_zone = &global.small_last;
    first_zone = &global.small_first;
    break;
  case LARGE:
    last_zone = &global.large_last;
    first_zone = &global.large_first;
    break;
  }
  if (*last_zone) {
    (*last_zone)->next = new_zone;
    new_zone->prev = *last_zone;
  }
  if (!(*first_zone)) {
    *first_zone = new_zone;
  }
  *last_zone = new_zone;
  printf("result first: %p\n", global.tiny_first);
  printf("result last: %p\n", global.tiny_last);
}

inline t_zone *newZone(const size_t size) {
  const enum HEAP_TYPE heap_type = getHeapType(size);
  // creating new zone
  debugInfo("creating new zone");
  t_zone *new_zone;
  if (heap_type == LARGE) {
    new_zone = mmap(NULL, size, MMAP_PROT, MMAP_FLAGS, -1, 0);
  } else {
    new_zone = newLimitedZone(heap_type);
  }
  if (new_zone == MAP_FAILED)
    return debugError("map failed bruhh"), NULL;
  // appending new zone to list of zones
  debugInfo("still creating new zone");
  initNewZone(new_zone, heap_type);
  appendNewZone(new_zone, heap_type);
  return new_zone;
}
