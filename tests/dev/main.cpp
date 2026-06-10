#include "../src/tester.cpp"
#include "ft_malloc.h"
#include <chrono>

#define SPEED_TEST_XXX_SIZE 1000

// testing allocs of all sizes

void base() {
  Tester tester = Tester();
  char *tiny1 = (char *)tester.wrap_malloc(sizeof(char) * 82, 0);
  char *tiny2 = (char *)tester.wrap_malloc(sizeof(char) * 107, 0);
  t_nest *tiny3 = (t_nest *)tester.wrap_malloc(sizeof(t_nest), 0);
  t_test *tiny4 = (t_test *)tester.wrap_malloc(sizeof(t_test), 0);
  char *small1 = (char *)tester.wrap_malloc(sizeof(char) * 651, 0);
  char *small2 = (char *)tester.wrap_malloc(sizeof(char) * 512, 0);
  char *small3 = (char *)tester.wrap_malloc(sizeof(char) * 817, 0);
  char *small4 = (char *)tester.wrap_malloc(sizeof(char) * 353, 0);
  tester.wrap_free(tiny2);
  tester.wrap_free(tiny3);
  tester.wrap_free(small3);
  tester.wrap_free(small1);
  tiny1 = (char *)tester.wrap_malloc(sizeof(char) * 80, tiny1);
  tiny2 = (char *)tester.wrap_malloc(sizeof(char) * 105, tiny2);
  tiny3 = (t_nest *)tester.wrap_malloc(sizeof(t_nest), tiny3);
  tiny4 = (t_test *)tester.wrap_malloc(sizeof(t_test), tiny4);
  small1 = (char *)tester.wrap_malloc(sizeof(char) * 651, small1);
  small2 = (char *)tester.wrap_malloc(sizeof(char) * 512, small2);
  small3 = (char *)tester.wrap_malloc(sizeof(char) * 817, small3);
  small4 = (char *)tester.wrap_malloc(sizeof(char) * 353, small4);
  tester.wrap_free(tiny2);
  tester.wrap_free(tiny3);
  tester.wrap_free(small3);
  tester.wrap_free(small1);
  tiny1 = (char *)tester.wrap_malloc(sizeof(char) * 80, tiny1);
  tiny2 = (char *)tester.wrap_malloc(sizeof(char) * 105, tiny2);
  show_alloc_mem();
}

void base1() {
  Tester tester = Tester();
  void *ptr = tester.wrap_malloc(80, 0);
  ptr = tester.wrap_malloc(80, ptr);
  tester.wrap_free(ptr);
}

inline static void
speedTest200(const size_t val) {
  void *addresses[SPEED_TEST_XXX_SIZE];
  for (int i = 0; i < SPEED_TEST_XXX_SIZE; i++) {
    void *ptr = ft_malloc(val);
    if (!ptr)
      throw runtime_error("malloc failed on speedTest100");
    addresses[i] = ptr;
  }
  for (int i = 0; i < SPEED_TEST_XXX_SIZE; i++) {
    ft_free(addresses[i]);
  }
}

template <typename T>
static void timeTest(const char *testName, T &&fn) {
  cout << testName << ": ";
  struct timespec start;
  clock_gettime(CLOCK_REALTIME, &start);
  fn();
  struct timespec end;
  clock_gettime(CLOCK_REALTIME, &end);
  long elapsed_ns = (end.tv_sec - start.tv_sec) * 1000000000L + (end.tv_nsec - start.tv_nsec);
  auto ms = std::chrono::milliseconds(elapsed_ns / 1000);
  cout << ms.count() << "µs" << endl;
}

int main() {
  //  base();
  speedTest200(70);
  timeTest("tinySpeed 1000", []() { speedTest200(10); });
  timeTest("tinySpeed 1000", []() { speedTest200(10); });
  timeTest("tinySpeed 1000", []() { speedTest200(10); });
  timeTest("tinySpeed 1000", []() { speedTest200(10); });
  timeTest("tinySpeed 1000", []() { speedTest200(10); });
  timeTest("tinySpeed 1000", []() { speedTest200(10); });
  timeTest("tinySpeed 1000", []() { speedTest200(10); });
  timeTest("tinySpeed 1000", []() { speedTest200(10); });
}
