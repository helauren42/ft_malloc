#include "malloc.h"
#include <stddef.h>
#include <unistd.h>

inline void debugInfo(char *str) {
  if (!DEV)
    return;
  write(STDOUT_FILENO, str, ft_strlen(str));
  write(STDOUT_FILENO, "\n", 1);
}

inline void debugError(char *str) {
  if (!DEV)
    return;
  write(STDERR_FILENO, "MALLOC ERROR: ", 14);
  write(STDERR_FILENO, str, ft_strlen(str));
  write(STDERR_FILENO, "\n", 1);
}

inline void debugVal(const char *text, const char *charname, const size_t var) {
  if (!DEV)
    return;
  printStr(text);
  if (ft_strlen(text) > 0)
    printStr(" - ");
  printStr(charname);
  printStr(": ");
  ft_putsize_t(var, 1);
  printLine("");
}

inline void debugAddr(const char *textAddrName, const void *ptr) {
  if (!DEV)
    return;
  printStr(textAddrName);
  printStr(": ");
  printAddr(ptr, true);
}

inline void printFreeChunks(const t_heap *heap) {
  return;
  t_free_chunk *chunk = heap->first_free_chunk;
  printStr("Free chunks for heap ");
  printAddr(heap, true);
  while (chunk) {
    printStr("  Chunk ");
    printAddr(chunk, true);
    printStr("    next:         ");
    printAddr(chunk->next, true);
    printStr("    prev:         ");
    printAddr(chunk->prev, true);
    printStr("    heap:         ");
    printAddr(chunk->heap, true);
    printStr("    payload_size: ");
    ft_putsize_t(chunk->payload_bytes, 1);
    printLine("");
    printStr("    is_free:      ");
    ft_putnbr_fd(chunk->is_free, 1);
    printLine("");
    printStr("    next_free:    ");
    printAddr(chunk->next_free, true);
    printStr("    prev_free:    ");
    printAddr(chunk->prev_free, true);
    chunk = chunk->next_free;
  }
}

inline void printHeapChunks(t_heap *heap) {
  printStr("Print Heap Chunks for heap: ");
  t_chunk *chunk = heap->first_chunk;
  while (chunk) {
    printStr("Chunk addr: ");
    printAddr(chunk, true);
    ft_putsize_t(chunk->payload_bytes, 1);
    printStr("\nIs free: ");
    printLine(chunk->is_free ? "true" : "false");
    chunk = chunk->next;
  }
}
