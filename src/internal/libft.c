#include "ft_malloc.h"

void ft_bzero(void *dst, const size_t n) {
  unsigned int i;

  i = 0;
  while (i < n) {
    ((char *)dst)[i] = 0;
    i++;
  }
}

int ft_strlen(const char *str) {
  int i;

  i = 0;
  while (str[i])
    i++;
  return (i);
}

char *ft_strcpy(char *dest, char *src) {
  int i;

  i = 0;
  while (src[i]) {
    dest[i] = src[i];
    i++;
  }
  dest[i] = src[i];
  return (dest);
}
