#include "../src/tester.cpp"
#include "ft_malloc.h"
#include <chrono>
#include <cstddef>
#include <cstdlib>
#include <ctime>

using TEST_TYPE = HEAP_TYPE; // used for randomization with switch case

inline static size_t getRandomValue(const size_t min, const size_t range) {
  static size_t seed = time(0);
  srand(seed);
  seed++;
  return min + (rand() % range);
}

inline static void
speedTestFuncRand(const size_t min, const size_t max, const size_t rounds) {
  const size_t range = max - min;
  for (int i = 0; i < rounds; i++) {
    const size_t val = getRandomValue(min, range);
    // ft_malloc(val);
    malloc(val);
  }
}

inline static void
speedTestFuncRaw(const size_t val, const size_t rounds) {
  for (int i = 0; i < rounds; i++) {
    ft_malloc(val);
    ft_free(ft_malloc(val));
    // malloc(val);
    // free(malloc(val));
  }
}

static void timeTest(const char *testName, void (*speedTestFunc)(const size_t val, const size_t rounds), const size_t val, const size_t rounds) {
  cout << "Timing " << testName << endl;
  struct timespec start;
  clock_gettime(CLOCK_REALTIME, &start);
  speedTestFunc(val, rounds);
  struct timespec end;
  clock_gettime(CLOCK_REALTIME, &end);
  long elapsed_ns = (end.tv_sec - start.tv_sec) * 1000000000L + (end.tv_nsec - start.tv_nsec);
  auto ms = std::chrono::milliseconds(elapsed_ns / 1000);
  cout << "Time: " << ms.count() << "µs" << endl;
}

int main() {
  timeTest("tinySpeed", speedTestFuncRaw, 50, 1000);
  // timeTest("smallSpeed", speedTestFunc, SMALL_MAX_PAYLOAD, SMALL_MAX_PAYLOAD, 1000);
  // show_alloc_mem();
  return 0;
}
