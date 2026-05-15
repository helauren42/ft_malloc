#ifndef FT_MALLOC
#define FT_MALLOC

#include <stdbool.h>
#include <stddef.h>
#include <sys/mman.h>
#include <unistd.h>

#define DEV 1
#define SECURE 0 // 0-1 if 1 then on free data will be zeroed

#define PAGE_SIZE getpagesize()
#define T_BLOCK_SIZE sizeof(t_block)
#define T_FREE_BLOCK_SIZE sizeof(t_free_block)
#define T_ZONE_SIZE sizeof(t_zone)
#define TINY_ZONE_SIZE (PAGE_SIZE * 8)
#define TINY_MAX_BYTES 128 - T_BLOCK_SIZE // 32768 / 128 = 256
#define SMALL_ZONE_SIZE (PAGE_SIZE * 64)
#define SMALL_MAX_BYTES 1024 - T_BLOCK_SIZE // 4096 * 64 / 1024 = 256

#define SMALL_MIN_BYTES TINY_MAX_BYTES + 1
#define LARGE_MIN_BYTES SMALL_MAX_BYTES + 1

#define MMAP_FLAGS (MAP_PRIVATE | MAP_ANONYMOUS | MAP_POPULATE)
#define MMAP_PROT (PROT_READ | PROT_WRITE)

#define PAYLOAD_SIZE(block_bytes) block_bytes - T_BLOCK_SIZE

enum HEAP_TYPE { TINY, SMALL, LARGE };

enum FUNCTION_CALLED { MALLOC, REALLOC, FREE };

typedef struct s_heaps t_heaps;
typedef struct s_block t_block;
typedef struct s_free_block t_free_block;
typedef struct s_zone t_zone;

typedef struct s_block {
  t_block *next;
  t_block *prev;
  t_zone *zone;
  size_t payload_bytes;
  bool is_free;
} t_block;

typedef struct s_free_block {
  t_block *next;
  t_block *prev;
  void *zone;
  size_t payload_bytes;
  bool is_free;
  t_free_block *next_free;
  t_free_block *prev_free;
} t_free_block;

typedef struct s_zone {
  enum HEAP_TYPE heap_type;
  t_zone *prev;
  t_zone *next;
  t_block *first_block;
  t_free_block *first_free_block;
  unsigned int active_block_count; // checked on freeing to know if zone should
                                   // be unmapped
  size_t size;
  size_t free_bytes; // check on allocation to know if the zone potentially
                     // has a big enough block left or not but actually
                     // first_free_block might be enough for this
} t_zone;

typedef struct s_heaps {
  t_zone *tiny_first;
  t_zone *small_first;
  t_zone *large_first;
  t_zone *tiny_last;
  t_zone *small_last;
  t_zone *large_last;
  enum FUNCTION_CALLED function_called;
} t_heaps;

extern t_heaps g_heaps;

// MAIN
void free(void *ptr);
void *ft_malloc(size_t size);
void *realloc(void *ptr, size_t size);
void show_alloc_mem_ex();

// ZONES
t_zone *newZone(const size_t size);
t_zone *getFirstZone(const enum HEAP_TYPE heap_type);

// BLOCKS
t_block *allocBlock(const size_t bytesNeeded);

// UTILS
enum HEAP_TYPE getHeapType(const size_t bytesNeeded);
t_zone *getHeapStart(const enum HEAP_TYPE heap_type);
void *getPayloadAddr(t_block *block);
t_block *getHeaderAddr(void *payload);

// ERRORS
void errorHeapMetadataCorruption();
void errorDoubleFree();

// DEBUGS
void debugError(char *str);
void debugInfo(char *str);

// LIBFT
int ft_strlen(const char *str);
void ft_bzero(void *dst, const size_t n);
void ft_putnbr_fd(long n, int fd);
void ft_putsize_t(size_t n, int fd);

// PRINT
void printLine(const char *str);
void printStr(const char *str);
void printAddr(void *addr);

#endif
