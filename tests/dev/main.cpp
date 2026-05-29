#include "../src/tester.cpp"
#include "ft_malloc.h"

void base() {
  Tester tester = Tester();
  for (int i = 0; i < 500; i++) {
    void *ptr = tester.wrap_malloc(2000, NULL);
    // tester.wrap_free(ptr);
  }
  show_alloc_mem();
}

int main() {
  base();
}
