#include "../includes/ft_malloc.h"
#include "../src/tester.cpp"
#include <chrono>
#include <cstddef>
#include <cstdlib>
#include <ctime>
#include <stdexcept>

#define SPEED_TEST_XXX_SIZE 1000

using TEST_TYPE = HEAP_TYPE; // used for randomization with switch case

inline static size_t getRandomValue(const size_t min, const size_t range) {
  static size_t seed = time(0);
  srand(seed);
  seed++;
  return min + (rand() % range);
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

inline static void
speedTestFuncRaw(const size_t val, const size_t rounds) {
  for (int i = 0; i < rounds; i++) {
    void *ptr1 = ft_malloc(val);
    void *ptr = ft_malloc(val);
    ft_free(ptr);
    ft_free(ptr1);
    if (!ptr1 || !ptr) {
      throw runtime_error("malloc failed");
    }
    // void *ptr1 = malloc(val);
    // void *ptr = malloc(val);
    // free(ptr);
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
  // Tester tester = Tester();
  // void *ptr = tester.wrap_malloc(80, 0);
  // ptr = tester.wrap_malloc(80, ptr);
  // tester.wrap_free(ptr);
  // TINY
  timeTest("tinySpeed raw", []() { speedTestFuncRaw(50, 10); });
  timeTest("tinySpeed raw", []() { speedTestFuncRaw(50, 1000); });
  timeTest("tinySpeed raw", []() { speedTestFuncRaw(50, 1000); });
  // timeTest("tinySpeed 1000", []() { speedTest200(0); });
  // // SMALL
  timeTest("smallSpeed raw", []() { speedTestFuncRaw(50, 1000); });
  // timeTest("smallSpeed 1000", []() { speedTest1000(80); });
  // show_alloc_mem();
  return 0;
}
