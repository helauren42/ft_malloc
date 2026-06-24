#include "../src/tester.cpp"
#include "ft_malloc.h"
#include <cstdlib>
#include <string.h>
#include <unistd.h>

#define WRITING_TEST_SIZE 20

int loops = 0;

void initString(char *dest, const char *val) {
  int i = 0;
  while (val[i]) {
    dest[i] = val[i];
    i++;
  }
}

void writing() {
  write(1, "here1\n", 7);
  const int alloc_size = 30;
  const char text[] = "hello world\n";
  Tester tester = Tester();
  char *mem[WRITING_TEST_SIZE];
  for (int i = 0; i < WRITING_TEST_SIZE; i++) {
    mem[i] = (char *)tester.wrap_malloc(alloc_size, 0);
    initString(mem[i], text);
    write(1, mem[i], 12);
  }
  show_alloc_mem();
}

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

void testRealloc() {
  Tester tester = Tester();
  char *ptr1 = (char *)tester.wrap_malloc(10, NULL);
  char *ptr2 = (char *)tester.wrap_malloc(10, NULL);
  char *ptr3 = (char *)tester.wrap_malloc(10, NULL);
  char *ptr4 = (char *)tester.wrap_malloc(10, NULL);
  for (int i = 0; i < 10; i++) {
    ptr1[i] = 'a';
    ptr2[i] = 'a';
    ptr3[i] = 'a';
    ptr4[i] = 'a';
  }
  ptr1 = (char *)tester.wrap_realloc(21, ptr1);
  ptr2 = (char *)tester.wrap_realloc(21, ptr2);
  ptr3 = (char *)tester.wrap_realloc(21, ptr3);
  ptr4 = (char *)tester.wrap_realloc(21, ptr4);
  const char expected[] = "aaaaaaaaaa";
  if (!strcmp(ptr1, expected)) {
    cout << RED << "Realloc error" << endl;
    exit(1);
  }
  if (!strcmp(ptr2, expected)) {
    cout << RED << "Realloc error" << endl;
    exit(1);
  }
  if (!strcmp(ptr3, expected)) {
    cout << RED << "Realloc error" << endl;
    exit(1);
  }
  if (!strcmp(ptr4, expected)) {
    cout << RED << "Realloc error" << endl;
    exit(1);
  }
}

int main() {
  base();
  writing();
  testRealloc();
  return 0;
}
