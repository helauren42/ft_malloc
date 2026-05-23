#include "ft_malloc.h"
#include <stddef.h>
#include <stdint.h>
#include <unistd.h>

static inline void printHex(const uint8_t num) {
  static const char *HEX[256] = {
      "00 ", "01 ", "02 ", "03 ", "04 ", "05 ", "06 ", "07 ", "08 ", "09 ",
      "0a ", "0b ", "0c ", "0d ", "0e ", "0f ", "10 ", "11 ", "12 ", "13 ",
      "14 ", "15 ", "16 ", "17 ", "18 ", "19 ", "1a ", "1b ", "1c ", "1d ",
      "1e ", "1f ", "20 ", "21 ", "22 ", "23 ", "24 ", "25 ", "26 ", "27 ",
      "28 ", "29 ", "2a ", "2b ", "2c ", "2d ", "2e ", "2f ", "30 ", "31 ",
      "32 ", "33 ", "34 ", "35 ", "36 ", "37 ", "38 ", "39 ", "3a ", "3b ",
      "3c ", "3d ", "3e ", "3f ", "40 ", "41 ", "42 ", "43 ", "44 ", "45 ",
      "46 ", "47 ", "48 ", "49 ", "4a ", "4b ", "4c ", "4d ", "4e ", "4f ",
      "50 ", "51 ", "52 ", "53 ", "54 ", "55 ", "56 ", "57 ", "58 ", "59 ",
      "5a ", "5b ", "5c ", "5d ", "5e ", "5f ", "60 ", "61 ", "62 ", "63 ",
      "64 ", "65 ", "66 ", "67 ", "68 ", "69 ", "6a ", "6b ", "6c ", "6d ",
      "6e ", "6f ", "70 ", "71 ", "72 ", "73 ", "74 ", "75 ", "76 ", "77 ",
      "78 ", "79 ", "7a ", "7b ", "7c ", "7d ", "7e ", "7f ", "80 ", "81 ",
      "82 ", "83 ", "84 ", "85 ", "86 ", "87 ", "88 ", "89 ", "8a ", "8b ",
      "8c ", "8d ", "8e ", "8f ", "90 ", "91 ", "92 ", "93 ", "94 ", "95 ",
      "96 ", "97 ", "98 ", "99 ", "9a ", "9b ", "9c ", "9d ", "9e ", "9f ",
      "a0 ", "a1 ", "a2 ", "a3 ", "a4 ", "a5 ", "a6 ", "a7 ", "a8 ", "a9 ",
      "aa ", "ab ", "ac ", "ad ", "ae ", "af ", "b0 ", "b1 ", "b2 ", "b3 ",
      "b4 ", "b5 ", "b6 ", "b7 ", "b8 ", "b9 ", "ba ", "bb ", "bc ", "bd ",
      "be ", "bf ", "c0 ", "c1 ", "c2 ", "c3 ", "c4 ", "c5 ", "c6 ", "c7 ",
      "c8 ", "c9 ", "ca ", "cb ", "cc ", "cd ", "ce ", "cf ", "d0 ", "d1 ",
      "d2 ", "d3 ", "d4 ", "d5 ", "d6 ", "d7 ", "d8 ", "d9 ", "da ", "db ",
      "dc ", "dd ", "de ", "df ", "e0 ", "e1 ", "e2 ", "e3 ", "e4 ", "e5 ",
      "e6 ", "e7 ", "e8 ", "e9 ", "ea ", "eb ", "ec ", "ed ", "ee ", "ef ",
      "f0 ", "f1 ", "f2 ", "f3 ", "f4 ", "f5 ", "f6 ", "f7 ", "f8 ", "f9 ",
      "fa ", "fb ", "fc ", "fd ", "fe ", "ff "};
  write(1, HEX[num], 3);
}

inline static void dumpPayload(const uint8_t *ptr, size_t len) {
  printStr("    Payload: ");
  for (size_t i = 0; i < len; i++) {
    printHex(*(ptr + i));
  }
  printStr("\n");
}

inline static void dumpHeader(const uint8_t *ptr) {
  printStr("\n    Header: ");
  for (size_t i = 0; i < T_CHUNK_SIZE; i++)
    printHex(*(ptr + i));
  printStr("\n");
}

inline static void hexDump(const t_chunk *chunk) {
  uint8_t *uint_ptr = (uint8_t *)chunk;
  dumpHeader(uint_ptr);
  dumpPayload(uint_ptr + T_CHUNK_SIZE, chunk->payload_bytes);
}

inline static void printChunkTitle(const unsigned int count) {
  write(1, "  ", 2);
  ft_putnbr_fd(count, 1);
  write(1, "=> ", 3);
}

inline static void printChunk(const unsigned int count, const t_chunk *chunk,
                              const bool hex) {
  printChunkTitle(count);
  printAddr(chunk, false);
  printStr(" - ");
  const void *lastByteAddr =
      (void *)chunk + T_CHUNK_SIZE + chunk->payload_bytes;
  printAddr(lastByteAddr, false);
  printStr(" : ");
  hex ? hexDump(chunk) : ft_putsize_t(chunk->payload_bytes, 1);
  printLine("");
}

inline static size_t printChunks(t_chunk *chunk, const bool hex) {
  unsigned int i = 0;
  size_t bytes_used = 0;
  if (!chunk)
    debugInfo("Chunk is null");
  else
    debugInfo("Chunk is not null");
  while (chunk) {
    // printAddr(chunk, true);
    if (!chunk->is_free) {
      bytes_used += chunk->payload_bytes;
      printChunk(i, chunk, hex);
      i++;
    }
    chunk = chunk->next;
  }
  return bytes_used;
}

inline void printHeapTitle(const unsigned int count) {
  write(1, "Heap", 4);
  ft_putnbr_fd(count, 1);
  write(1, "=> ", 3);
}

inline size_t getLargeHeapSize(t_heap *heap) {
  size_t usedSize = 0;
  t_chunk *chunk = heap->first_chunk;
  while (chunk) {
    usedSize += T_CHUNK_SIZE + chunk->payload_bytes;
  }
  size_t heapSize = PAGE_SIZE;
  while (heapSize < usedSize)
    heapSize += PAGE_SIZE;
  return heapSize;
}

inline size_t getHeapSize(t_heap *heap, const enum HEAP_TYPE heap_type) {
  switch (heap_type) {
  case TINY:
    return TINY_HEAP_SIZE;
  case SMALL:
    return SMALL_HEAP_SIZE;
  case LARGE:
    return getLargeHeapSize(heap);
  }
  return 0;
}

inline t_mem_usage printHeaps(t_heap *heap, const enum HEAP_TYPE heap_type,
                              const bool hex) {
  // TODO
  unsigned int i = 0;
  size_t bytes_used = 0;
  size_t bytes_mapped = 0;
  while (heap) {
    printHeapTitle(i);
    printAddr(heap, true);
    bytes_used += printChunks(heap->first_chunk, hex);
    bytes_mapped += getHeapSize(heap, heap_type);
    heap = heap->next;
    i++;
  }
  t_mem_usage mem = {bytes_mapped, bytes_used};
  return mem;
}

inline t_mem_usage addMem(const t_mem_usage a, const t_mem_usage b) {
  const t_mem_usage total = {a.bytes_mapped + b.bytes_mapped,
                             a.bytes_used + b.bytes_used};
  return total;
}
