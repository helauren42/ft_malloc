#include "ft_malloc.h"
#include <stdio.h>
#include <string.h>

int main() {
  char *str = ft_malloc(sizeof(char) * 50);
  strcpy(str, "Hello World");
  printf("%s\n", str);
  printf("%p\n", str);
  char *s2 = ft_malloc(sizeof(char) * 250);
  strcpy(s2, "s2 Hello World");
  printf("expecting %p\n", (void *)str - T_CHUNK_SIZE);
  printf("expecting %p\n", (void *)s2 - T_CHUNK_SIZE);
  show_alloc_mem();
  return 0;
}
