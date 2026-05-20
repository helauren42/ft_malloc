#include "ft_malloc.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct s_mem_usage t_mem_usage;

typedef struct s_mem_usage {
  size_t bytes_mapped;
  size_t bytes_used;
} t_mem_usage;

inline static void printBlockTitle(const unsigned int count) {
  write(1, "  ", 2);
  ft_putnbr_fd(count, 1);
  write(1, ": ", 2);
}

inline static void printBlock(const unsigned int count, const t_block *block) {
  printBlockTitle(count);
  printAddr(block, false);
  printStr(" - ");
  printAddr(block + block->payload_bytes, false);
  printStr(" : ");
  ft_putsize_t(block->payload_bytes, 1);
  printLine("");
}

inline static size_t printBlocks(t_block *block) {
  printLine("printBlocks!!");
  unsigned int i = 0;
  size_t bytes_used = 0;
  if (!block)
    printLine("Block is null");
  else
    printLine("Block is not null");
  while (block) {
    // printAddr(block, true);
    if (!block->is_free) {
      bytes_used += block->payload_bytes;
      printBlock(i, block);
      i++;
    }
    block = block->next;
  }
  return bytes_used;
}

inline static void printZoneTitle(const unsigned int count) {
  write(1, "Zone", 4);
  ft_putnbr_fd(count, 1);
  write(1, ": ", 2);
}

inline static size_t getLargeZoneSize(t_zone *zone) {
  size_t usedSize = 0;
  t_block *block = zone->first_block;
  while (block) {
    usedSize += T_BLOCK_SIZE + block->payload_bytes;
  }
  size_t zoneSize = PAGE_SIZE;
  while (zoneSize < usedSize)
    zoneSize += PAGE_SIZE;
  return zoneSize;
}

inline static size_t getZoneSize(t_zone *zone, const enum HEAP_TYPE heap_type) {
  switch (heap_type) {
  case TINY:
    return TINY_ZONE_SIZE;
  case SMALL:
    return SMALL_ZONE_SIZE;
  case LARGE:
    return getLargeZoneSize(zone);
  }
  return 0;
}

inline static t_mem_usage printZones(t_zone *zone,
                                     const enum HEAP_TYPE heap_type) {
  // TODO
  unsigned int i = 0;
  size_t bytes_used = 0;
  size_t bytes_mapped = 0;
  while (zone) {
    printZoneTitle(i);
    printAddr(zone, true);
    printLine("PRE!!!");
    bytes_used += printBlocks(zone->first_block);
    printLine("POST!!!");
    bytes_mapped += getZoneSize(zone, heap_type);
    zone = zone->next;
    i++;
  }
  t_mem_usage mem = {bytes_mapped, bytes_used};
  return mem;
}

inline static t_mem_usage addMem(const t_mem_usage a, const t_mem_usage b) {
  const t_mem_usage total = {a.bytes_mapped + b.bytes_mapped,
                             a.bytes_used + b.bytes_used};
  return total;
}

void show_alloc_mem() {
  t_mem_usage totalMem = {0, 0};
  t_mem_usage toAdd = {0, 0};

  printLine("------------ TINY HEAP ------------");
  toAdd = printZones(g_global.tiny_first, TINY);
  totalMem = addMem(totalMem, toAdd);
  printLine("------------ SMALL HEAP ------------");
  toAdd = printZones(g_global.small_first, SMALL);
  totalMem = addMem(totalMem, toAdd);
  printLine("------------ LARGE HEAP ------------");
  toAdd = printZones(g_global.large_first, LARGE);
  totalMem = addMem(totalMem, toAdd);
  printLine("------------ HEAP END ------------");
  printStr("Total Bytes Mapped: ");
  ft_putsize_t(totalMem.bytes_mapped, 1);
  printStr("\n");
  printStr("Total Bytes Used: ");
  ft_putsize_t(totalMem.bytes_used, 1);
  printStr("\n");
}
