#include "ft_malloc.h"

void ft_bzero(void *dst, size_t n) {
  unsigned int i;

  i = 0;
  while (i < n) {
    ((char *)dst)[i] = 0;
    i++;
  }
}

int ft_strlen(char *str) {
  int i;

  i = 0;
  while (str[i])
    i++;
  return (i);
}
