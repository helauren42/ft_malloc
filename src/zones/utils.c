#include "ft_malloc.h"

inline t_zone *getLastZone(t_zone *zone) {
  while (zone && zone->next)
    zone = zone->next;
  return zone;
}

inline t_zone *getFirstZone(const enum HEAP_TYPE heap_type) {
  switch (heap_type) {
  case TINY:
    return global.tiny_heap;
  case SMALL:
    return global.small_heap;
  default:
    return global.large_heap;
  }
}
