#include "../src/tester.cpp"
#include "ft_malloc.h"

void base() {
  Tester tester = Tester();

  /* Tiny allocations */
  char *tiny1 = (char *)tester.wrap_malloc(sizeof(char) * 82, 0);
  char *tiny2 = (char *)tester.wrap_malloc(sizeof(char) * 107, 0);
  t_nest *tiny3 = (t_nest *)tester.wrap_malloc(sizeof(t_nest), 0);
  t_test *tiny4 = (t_test *)tester.wrap_malloc(sizeof(t_test), 0);

  /* Small allocations */
  char *small1 = (char *)tester.wrap_malloc(sizeof(char) * 651, 0);
  char *small2 = (char *)tester.wrap_malloc(sizeof(char) * 512, 0);
  char *small3 = (char *)tester.wrap_malloc(sizeof(char) * 817, 0);
  char *small4 = (char *)tester.wrap_malloc(sizeof(char) * 353, 0);

  // /* Large allocations (few thousands) */
  // char *large1 = (char *)tester.wrap_malloc(2048, 0);
  // char *large2 = (char *)tester.wrap_malloc(4096, 0);
  // char *large3 = (char *)tester.wrap_malloc(8192, 0);
  // char *large4 = (char *)tester.wrap_malloc(3072, 0);
  // char *large5 = (char *)tester.wrap_malloc(6144, 0);

  /* First round of frees */
  tester.wrap_free(tiny2);
  tester.wrap_free(tiny3);
  tester.wrap_free(small3);
  tester.wrap_free(small1);
  // tester.wrap_free(large2);
  // tester.wrap_free(large4);

  /* Re-allocations (resize/reuse) */
  tiny1 = (char *)tester.wrap_malloc(sizeof(char) * 80, tiny1);
  tiny2 = (char *)tester.wrap_malloc(sizeof(char) * 105, tiny2);
  tiny3 = (t_nest *)tester.wrap_malloc(sizeof(t_nest), tiny3);
  tiny4 = (t_test *)tester.wrap_malloc(sizeof(t_test), tiny4);

  small1 = (char *)tester.wrap_malloc(sizeof(char) * 651, small1);
  small2 = (char *)tester.wrap_malloc(sizeof(char) * 512, small2);
  small3 = (char *)tester.wrap_malloc(sizeof(char) * 817, small3);
  small4 = (char *)tester.wrap_malloc(sizeof(char) * 353, small4);

  // large1 = (char *)tester.wrap_malloc(4096, large1);
  // large2 = (char *)tester.wrap_malloc(3072, large2);
  // large3 = (char *)tester.wrap_malloc(16384, large3);
  // large4 = (char *)tester.wrap_malloc(4096, large4);
  // large5 = (char *)tester.wrap_malloc(8192, large5);

  /* Second round of frees */
  tester.wrap_free(tiny2);
  tester.wrap_free(tiny3);
  tester.wrap_free(small3);
  tester.wrap_free(small1);
  // tester.wrap_free(large1);
  // tester.wrap_free(large3);
  // tester.wrap_free(large5);

  /* Final re-allocations */
  tiny1 = (char *)tester.wrap_malloc(sizeof(char) * 80, tiny1);
  tiny2 = (char *)tester.wrap_malloc(sizeof(char) * 105, tiny2);
  tiny3 = (t_nest *)tester.wrap_malloc(sizeof(t_nest), tiny3);
  tiny4 = (t_test *)tester.wrap_malloc(sizeof(t_test), tiny4);

  small1 = (char *)tester.wrap_malloc(sizeof(char) * 651, small1);
  small2 = (char *)tester.wrap_malloc(sizeof(char) * 512, small2);
  small3 = (char *)tester.wrap_malloc(sizeof(char) * 817, small3);
  small4 = (char *)tester.wrap_malloc(sizeof(char) * 353, small4);

  // large1 = (char *)tester.wrap_malloc(8192, large1);
  // large2 = (char *)tester.wrap_malloc(8192, large2);
  // large3 = (char *)tester.wrap_malloc(16384, large3);
  // large4 = (char *)tester.wrap_malloc(6144, large4);
  // large5 = (char *)tester.wrap_malloc(12288, large5);

  show_alloc_mem();
}

int main() {
  base();
  return 0;
}
