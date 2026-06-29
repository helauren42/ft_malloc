#include "malloc.h"
#include <stdio.h>
#include <string.h>

int main() {
  char *str = (char *)ft_malloc(sizeof(char) * 20);
  char *str1 = (char *)ft_malloc(sizeof(char) * 160);
  char *str2 = (char *)ft_malloc(sizeof(char) * 320);
  strcpy(str, "Hello World");
  strcpy(str2, "str2 Hello World");
  printf("%s\n", str);
  printf("%s\n", str2);
  char *s2 = (char *)ft_malloc(sizeof(char) * 25);
  ft_free(str1);
  ft_free(str);
  ft_free(str2);
  ft_free(str2);
  return 0;
}
