#include "../src/tester.cpp"
#include "malloc.h"
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

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

inline static void speedTest200(const size_t val) {
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

void testRealloc1() {
  Tester tester = Tester();
  char *ptr1 = (char *)tester.wrap_malloc(10, NULL);
  char *ptr2 = (char *)tester.wrap_malloc(10, NULL);
  // char *ptr3 = (char *)tester.wrap_malloc(10, NULL);
  // char *ptr4 = (char *)tester.wrap_malloc(10, NULL);
  cout << "1 ADDR: " << hex << (uintptr_t)ptr1 - T_CHUNK_SIZE << endl;
  cout << "2 ADDR: " << hex << (uintptr_t)ptr2 - T_CHUNK_SIZE << endl;
  // cout << "3 ADDR: " << hex << (uintptr_t)ptr3 - T_CHUNK_SIZE << endl;
  // cout << "4 ADDR: " << hex << (uintptr_t)ptr4 - T_CHUNK_SIZE << endl;
  for (int i = 0; i < 10; i++)
    ptr2[i] = 'a';
  cout << "PRE: " << ptr2 << endl;
  ptr2 = (char *)tester.wrap_realloc(21, ptr2);
  cout << "POST: " << ptr2 << endl;
  cout << "NEW ADDR: " << hex << (uintptr_t)ptr2 - T_CHUNK_SIZE << endl;
}

void reallocCmp(const char *ptr, const char *expected) {
  for (int i = 0; ptr[i] || expected[i]; i++) {
    if (ptr[i] != expected[i]) {
      cout << RED << "Realloc error: " << endl;
      cout << "recv: " << ptr << endl;
      cout << "expc: " << ptr << endl;
      exit(1);
    }
  }
}

void testRealloc2() {
  Tester tester = Tester();
  char *ptr1 = (char *)tester.wrap_malloc(10, NULL);
  cout << "1 malloc: " << hex << (uintptr_t)ptr1 << endl;
  char *ptr2 = (char *)tester.wrap_malloc(10, NULL);
  char *ptr3 = (char *)tester.wrap_malloc(10, NULL);
  char *ptr4 = (char *)tester.wrap_malloc(10, NULL);
  for (int i = 0; i < 10; i++) {
    ptr1[i] = 'a';
    ptr2[i] = 'a';
    ptr3[i] = 'a';
    ptr4[i] = 'a';
  }
  cout << "\n\n\n\n\n" << "pre realloc" << endl;
  ptr1 = (char *)tester.wrap_realloc(21, ptr1);
  ptr2 = (char *)tester.wrap_realloc(21, ptr2);
  ptr3 = (char *)tester.wrap_realloc(21, ptr3);
  ptr4 = (char *)tester.wrap_realloc(21, ptr4);
  const char expected[] = "aaaaaaaaaa";
  reallocCmp(ptr1, expected);
  reallocCmp(ptr2, expected);
  reallocCmp(ptr3, expected);
  reallocCmp(ptr4, expected);
}

void test1() {
  for (int i = 0; i < 1024; i++) {
    void *ptr = ft_malloc(130);
  }
}

void big() {
  void *ptr1 = ft_malloc(16 * 1024 * 1024);
  // for (int i = 0; i > 0; i++) {
  //   if (i == 1000000)
  //     i = 0;
  // }
  // void *ptr2 = ft_malloc(1024 * 1024 * 1024); // 1 GB
}

int main() {
  big();
  //   test1();
  // testRealloc1();
  // testRealloc2();
  //  base();
  // speedTest200(70);
  // timeTest("tinySpeed 1000", []() { speedTest200(10); });
  // timeTest("tinySpeed 1000", []() { speedTest200(10); });
  // timeTest("tinySpeed 1000", []() { speedTest200(10); });
  // timeTest("tinySpeed 1000", []() { speedTest200(10); });
  // timeTest("tinySpeed 1000", []() { speedTest200(10); });
  // timeTest("tinySpeed 1000", []() { speedTest200(10); });
  // timeTest("tinySpeed 1000", []() { speedTest200(10); });
  // timeTest("tinySpeed 1000", []() { speedTest200(10); });
}
