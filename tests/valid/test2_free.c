#include "ft_malloc.h"
#include <stdio.h>
#include <string.h>

// testing allocs of all sizes

typedef struct s_nest {
  void *anything_else;
  void *something_else;
  char d;
  int a;
} t_nest;

typedef struct s_test {
  t_nest *nest;
  void *anything;
  void *something;
  char c;
  int b;
} t_test;

int main() {
  char *str = ft_malloc(sizeof(char) * 20);
  strcpy(str, "Hello World");
  printf("%s\n", str);
  printf("%p\n", str);
  char *s2 = ft_malloc(sizeof(char) * 250);
  strcpy(s2, "s2 Hello World");
  printf("expecting %p\n", (void *)str - T_BLOCK_SIZE);
  printf("expecting %p\n", (void *)s2 - T_BLOCK_SIZE);
  t_nest *nest = ft_malloc(sizeof(t_nest));
  t_test *test = ft_malloc(sizeof(t_test));
  void *v = ft_malloc(sizeof(t_test) + 300);
  free(nest);
  free(s2);
  show_alloc_mem();
  return 0;
}
