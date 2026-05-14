#include "ft_malloc.h"
#include <stdio.h>
#include <string.h>

int main() {
  char *str = ft_malloc(sizeof(char) * 20);
  strcpy(str, "Hello World");
  printf("%s\n", str);
  printf("%p\n", str);
  char *s2 = ft_malloc(sizeof(char) * 20);
  strcpy(s2, "s2 Hello World");
  printf("%s\n", s2);
  printf("%p\n", s2);
  return 0;
}
