#include "ft_malloc.h"
#include <unistd.h>

void *malloc(size_t size) {
  write(STDOUT_FILENO, "Called malloc", ft_strlen("Called malloc"));
  void *addr = NULL;
  return addr;
}
