#include "ft_malloc.h"
#include <stdio.h>
#include <string.h>

int main() {
  char *str = ft_malloc(sizeof(char) * 20);
  strcpy(str, "Hello World");
  printf("%s\n", str);
  printf("%p\n", str);
  char *s2 = ft_malloc(sizeof(char) * 25);
  strcpy(s2, "s2 Hello World");
  ft_free(s2);
  ft_free(s2);
  show_alloc_mem();
  return 0;
}
