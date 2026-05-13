#include "ft_malloc.h"
#include <stddef.h>
#include <unistd.h>

t_block *unfreeBlock(t_free_block *free_block) {}

t_block *getBlock(const size_t bytesNeeded) {
  const enum HEAP_TYPE heap_type = getZoneType(bytesNeeded);
  t_zone *firstZone = getFirstZone(heap_type);
  if (!firstZone && !newZone(bytesNeeded) ||
      (firstZone = getFirstZone(heap_type)))
    return NULL;
  // TODO
  t_free_block *free_block = firstZone->first_free_block;
  while (free_block && free_block->next) {
    if (free_block->bytes >= bytesNeeded)
      return unfreeBlock(free_block);
  }
  return NULL;
}
