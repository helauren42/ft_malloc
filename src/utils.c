#include "ft_malloc.h"
#include <stddef.h>

enum ZONE_TYPE getZoneType(const size_t bytesNeeded) {
  if (bytesNeeded <= TINY_MAX_BYTES)
    return TINY;
  else if (bytesNeeded < SMALL_MAX_BYTES)
    return SMALL;
  return LARGE;
}
