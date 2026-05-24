#include "ft_malloc.h"
#include <stdio.h>
#include <string.h>

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
  char *s2 = ft_malloc(sizeof(char) * 25);
  strcpy(s2, "s2 Hello World");
  printf("expecting %p\n", (void *)str - T_CHUNK_SIZE);
  printf("expecting %p\n", (void *)s2 - T_CHUNK_SIZE);
  t_nest *nest = ft_malloc(sizeof(t_nest));
  ft_free(nest);
  ft_free(s2);
  show_alloc_mem();
  return 0;
}
