#include "ft_malloc.h"
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
  char *s2 = ft_malloc(sizeof(char) * 250);
  strcpy(s2, "s2 Hello World");
  t_nest *nest = ft_malloc(sizeof(t_nest));
  t_test *test = ft_malloc(sizeof(t_test));
  ft_free(nest);
  ft_free(s2);
  void *v = ft_malloc(sizeof(t_test) + 300);
  char *s3 = ft_malloc(sizeof(char) * 400);
  char *s4 = ft_malloc(sizeof(char) * 280);
  ft_free(s3);
  char *s5 = ft_malloc(sizeof(char) * 500);
  show_alloc_mem();
  return 0;
}
