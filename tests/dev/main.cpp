#include "../src/tester.cpp"
#include "ft_malloc.h"

// testing allocs of all sizes

void base1() {
  Tester tester = Tester();
  void *ptr = tester.wrap_malloc(5000, 0);
  ptr = tester.wrap_malloc(5000, ptr);
  tester.wrap_free(ptr);
}

int main() {
  base1();
}
