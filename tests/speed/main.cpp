#include "../includes/ft_malloc.h"
#include "../src/tester.cpp"
#include <chrono>
#include <cstddef>
#include <cstdlib>
#include <ctime>
#include <stdexcept>

#define SPEED_TEST_XXX_SIZE 1000

int loops = -1;

using TEST_TYPE = HEAP_TYPE; // used for randomization with switch case

inline static size_t getRandomValue(const size_t min, const size_t range) {
  static size_t seed = time(0);
  srand(seed);
  seed++;
  return min + (rand() % range);
}

inline static void speedTestXXX(const size_t payloadSize) {
  void *addresses[SPEED_TEST_XXX_SIZE];
  for (unsigned int i = 0; i < SPEED_TEST_XXX_SIZE; i++) {
    void *ptr = ft_malloc(payloadSize);
    if (!ptr)
      throw runtime_error("malloc failed on speedTest100");
    addresses[i] = ptr;
  }
  for (int i = 0; i < SPEED_TEST_XXX_SIZE; i++) {
    ft_free(addresses[i]);
  }
}

inline static void speedTestFuncRaw(const size_t val, const size_t rounds) {
  ft_malloc(val);
  for (loops = 0; loops < rounds; loops++) {
    void *ptr1 = ft_malloc(val);
    void *ptr = ft_malloc(val);
    // cout << "global's first_free_chunk: " << g_global.tiny_first->first_free_chunk << endl;
    // ft_free(ptr1);
    ft_free(ptr);
    if (!ptr1 || !ptr) {
      throw runtime_error("malloc failed");
    }
    // printHeapChunks(g_global.tiny_first);
    // cout << "global's first_free_chunk: " << g_global.tiny_first->first_free_chunk << endl;
    // void *ptr1 = malloc(val);
    // void *ptr = malloc(val);
    // free(ptr);
  }
}

template <typename T> static void timeTest(const char *testName, T &&fn) {
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
  timeTest("tinySpeed raw", []() { speedTestFuncRaw(50, 1000); });
  timeTest("tinySpeed raw", []() { speedTestFuncRaw(50, 1000); });
  timeTest("tinySpeed raw", []() { speedTestFuncRaw(50, 1000); });
  timeTest("tinySpeed XXX", []() { speedTestXXX(256); });
  timeTest("tinySpeed XXX", []() { speedTestXXX(256); });
  timeTest("tinySpeed XXX", []() { speedTestXXX(256); });
  // timeTest("tinySpeed1000", []() { speedTest200(10); });
  // // SMALL
  timeTest("smallSpeed raw", []() { speedTestFuncRaw(50, 1000); });
  timeTest("smallSpeed raw", []() { speedTestFuncRaw(50, 1000); });
  timeTest("smallSpeed raw", []() { speedTestFuncRaw(50, 1000); });
  timeTest("smallSpeed XXX", []() { speedTestXXX(0); });
  timeTest("smallSpeed XXX", []() { speedTestXXX(0); });
  timeTest("smallSpeed XXX", []() { speedTestXXX(0); });
  // show_alloc_mem();
  return 0;
}
