#ifndef FT_MALLOC
#define FT_MALLOC

#include <stdbool.h>
#include <sys/mman.h>
#include <unistd.h>

#define SECURE 0 // 0-1 if 1 then on free data will be zeroed
#define PAGE_SIZE getpagesize()
#define T_BLOCK_SIZE sizeof(t_block)
#define T_ZONE_SIZE sizeof(t_zone)

#define TINY_ZONE_SIZE (PAGE_SIZE * 8)
#define TINY_MAX_BYTES 128 - T_BLOCK_SIZE // 32768 / 128 = 256
#define SMALL_ZONE_SIZE (PAGE_SIZE * 64)
#define SMALL_MAX_BYTES 1024 - T_BLOCK_SIZE // 4096 * 64 / 1024 = 256
#define BLOCK_MIN_SIZE sizeof(t_free_block)

#define MMAP_FLAGS (MAP_PRIVATE | MAP_ANONYMOUS | MAP_POPULATE)
#define MMAP_PROT (PROT_READ | PROT_WRITE)

enum HEAP_TYPE { TINY, SMALL, LARGE };

typedef struct s_global t_global;

typedef struct s_block t_block;

typedef struct s_block {
  t_block *next;
  t_block *prev;
  size_t bytes;
  bool freed;
} t_block;
// used by hashmap
typedef struct s_free_block t_free_block;

typedef struct s_free_block {
  t_block *next;
  t_block *prev;
  size_t bytes;
  bool freed;
  t_free_block *next_free;
  t_free_block *prev_free;
} t_free_block;

typedef struct s_zone t_zone;

typedef struct s_zone {
  t_zone *prev;
  t_zone *next;
  t_block *first_block;
  t_free_block *first_free_block;
  unsigned int
      block_count; // checked on freeing to know if zone should be unmapped
  size_t available_bytes; // check on allocation to know if the zone potentially
                          // has a big enough block left or not but actually
                          // first_free_block might be enough for this
} t_zone;

typedef struct s_global {
  t_zone *tiny_heap;
  t_zone *small_heap;
  t_zone *large_heap;
} t_global;

extern t_global global;

// MAIN
void ft_free(void *ptr);
void *malloc(size_t size);
void *ft_realloc(void *ptr, size_t size);

// ZONES
t_zone *newZone(const size_t size);
t_zone *getLastZone(t_zone *zone);
t_zone *getFirstZone(const enum HEAP_TYPE zone_type);

// UTILS
enum HEAP_TYPE getZoneType(size_t bytesNeeded);

// LIBFT
int ft_strlen(char *str);
void ft_bzero(void *dst, size_t n);

#endif
