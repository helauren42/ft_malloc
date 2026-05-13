#include "ft_malloc.h"
#include <stddef.h>

size_t mapIndex(void *ptr) {
  size_t value = (size_t)ptr;
  return value % 200;
}
