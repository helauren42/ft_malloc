#include "ft_malloc.h"
#include <stdio.h>
#include <string.h>

typedef struct s_test {
  void *anything;
  void *something;
  char c;
  int b;
} t_test;

// testing allocs of all sizes
int main() {
  char *str = ft_malloc(sizeof(char) * 20);
  strcpy(str, "Hello World");
  printf("%s\n", str);
  printf("%p\n", str);
  char *s2 = ft_malloc(sizeof(char) * 25);
  strcpy(s2, "s2 Hello World");
  printf("expecting %p\n", (void *)str - T_BLOCK_SIZE);
  printf("expecting %p\n", (void *)s2 - T_BLOCK_SIZE);
  t_test *test = ft_malloc(sizeof(t_test));
  show_alloc_mem_ex();
  return 0;
}
