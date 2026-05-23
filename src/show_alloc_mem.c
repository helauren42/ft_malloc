#include "ft_malloc.h"

void show_alloc_mem() {
  t_mem_usage totalMem = {0, 0};
  t_mem_usage toAdd = {0, 0};

  printLine("------------ TINY HEAP ------------");
  toAdd = printHeaps(g_global.tiny_first, TINY, false);
  totalMem = addMem(totalMem, toAdd);
  printLine("------------ SMALL HEAP ------------");
  toAdd = printHeaps(g_global.small_first, SMALL, false);
  totalMem = addMem(totalMem, toAdd);
  printLine("------------ LARGE HEAP ------------");
  toAdd = printHeaps(g_global.large_first, LARGE, false);
  totalMem = addMem(totalMem, toAdd);
  printLine("------------ HEAP END --------------");
  printStr("Total Bytes Mapped: ");
  ft_putsize_t(totalMem.bytes_mapped, 1);
  printStr("\n");
  printStr("Total Bytes Used: ");
  ft_putsize_t(totalMem.bytes_used, 1);
  printStr("\n");
}
