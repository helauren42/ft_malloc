#include "../src/tester.cpp"
#include "ft_malloc.h"

void base() {
  Tester tester = Tester();
  for (int i = 0; i < 500; i++) {
    void *ptr1 = tester.wrap_malloc(50, NULL);
    void *ptr2 = tester.wrap_malloc(50, NULL);
    if (!ptr1 || !ptr2) {
      throw runtime_error("malloc failed");
    }
  }
  show_alloc_mem();
}

int main() {
  base();
}
