#include "malloc.h"

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

void *ft_memcpy(void *dst, const void *src, size_t n) {
  size_t i;
  unsigned char *udst;
  unsigned char *usrc;

  i = 0;
  udst = (unsigned char *)dst;
  usrc = (unsigned char *)src;
  if (!usrc)
    return (udst);
  if (!udst)
    return (NULL);
  if (n == 0)
    return (dst);
  while (i < n) {
    udst[i] = usrc[i];
    i++;
  }
  return udst;
}
