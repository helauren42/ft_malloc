#include "ft_malloc.h"
#include <stdio.h>

int main() {
  printf("Testing SMALL allocation...\n");
  void *p1 = ft_malloc(500);
  if (p1)
    printf("p1 (500) success: %p\n", p1);
  else
    printf("p1 (500) failed\n");

  printf("\nTesting LARGE allocation...\n");
  void *p2 = ft_malloc(2000);
  if (p2)
    printf("p2 (2000) success: %p\n", p2);
  else
    printf("p2 (2000) failed\n");

  return 0;
}
