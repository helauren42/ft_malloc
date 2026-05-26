#include "../src/tester.cpp"
#include "ft_malloc.h"
#include <chrono>
#include <cstddef>
#include <cstdlib>
#include <ctime>
#include <functional>

using TEST_TYPE = HEAP_TYPE; // used for randomization with switch case

inline static size_t getRandomValue(const size_t min, const size_t range) {
  static size_t seed = time(0);
  srand(seed);
  seed++;
  return min + (rand() % range);
}

inline static void
speedTest1000(const size_t val) {
  void *addresses[1000];
  for (int i = 0; i < 1000; i++) {
    void *ptr = ft_malloc(val);
    addresses[i] = ptr;
  }
  for (int i = 0; i < 1000; i++) {
    ft_free(addresses[i]);
  }
}

inline static void
speedTestFuncRaw(const size_t val, const size_t rounds) {
  for (int i = 0; i < rounds; i++) {
    void *ptr1 = ft_malloc(val);
    void *ptr = ft_malloc(val);
    ft_free(ptr);
  }
}

static void timeTest(const char *testName, const function<void()> &fn) {
  cout << "Timing " << testName << endl;
  struct timespec start;
  clock_gettime(CLOCK_REALTIME, &start);
  fn();
  struct timespec end;
  clock_gettime(CLOCK_REALTIME, &end);
  long elapsed_ns = (end.tv_sec - start.tv_sec) * 1000000000L + (end.tv_nsec - start.tv_nsec);
  auto ms = std::chrono::milliseconds(elapsed_ns / 1000);
  cout << "Time: " << ms.count() << "µs" << endl;
}

int main() {
  timeTest("tinySpeed raw", []() { speedTestFuncRaw(50, 1000); });
  timeTest("tinySpeed 1000", []() { speedTest1000(80); });
  // timeTest("smallSpeed", speedTestFunc, SMALL_MAX_PAYLOAD, SMALL_MAX_PAYLOAD, 1000);
  // show_alloc_mem();
  return 0;
}
