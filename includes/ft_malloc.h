#ifdef __cplusplus
extern "C" {
#endif

#ifndef FT_MALLOC
#define FT_MALLOC

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/mman.h>
#include <unistd.h>

extern int loops;

#define DEV 0
#define SECURE 0 // 0-1 if 1 then on free data will be zeroed

#define PAGE_SIZE getpagesize()
#define T_CHUNK_SIZE sizeof(t_chunk) // 40 bytes
#define T_FREE_CHUNK_SIZE sizeof(t_free_chunk)
#define T_HEAP_SIZE sizeof(t_heap)
#define TINY_HEAP_SIZE (PAGE_SIZE * 6)
#define TINY_MAX_PAYLOAD 128
// max number of chunks is TINY_HEAP_SIZE / (TINY_MAX_PAYLOAD +
// T_CHUNK_SIZE) ~ 24576 / (128 + 40) ~ 146
#define SMALL_HEAP_SIZE (PAGE_SIZE * 26) // 104
#define SMALL_MAX_PAYLOAD 1024
#define GUARD_VAL 17496424073816618564

#define SMALL_MIN_PAYLOAD TINY_MAX_PAYLOAD + 1
#define LARGE_MIN_PAYLOAD SMALL_MAX_PAYLOAD + 1

#define MMAP_FLAGS (MAP_PRIVATE | MAP_ANONYMOUS | MAP_POPULATE)
#define MMAP_PROT (PROT_READ | PROT_WRITE)

#define PAYLOAD_SIZE(chunk_bytes) chunk_bytes - T_CHUNK_SIZE

enum HEAP_TYPE { TINY,
                 SMALL,
                 LARGE };

enum FUNCTION_CALLED { MALLOC,
                       REALLOC,
                       FREE };

typedef struct s_chunk t_chunk;
typedef struct s_free_chunk t_free_chunk;
typedef struct s_heap t_heap;
typedef struct s_arenas t_arenas;

typedef struct s_chunk {
  t_chunk *next;
  t_chunk *prev;
  t_heap *heap;
  size_t payload_bytes;
  bool is_free;
} t_chunk;

typedef struct s_free_chunk {
  t_chunk *next;
  t_chunk *prev;
  t_heap *heap;
  size_t payload_bytes;
  bool is_free;
  t_free_chunk *next_free;
  t_free_chunk *prev_free;
} t_free_chunk;

typedef struct s_heap {
  enum HEAP_TYPE heap_type;
  t_heap *prev;
  t_heap *next;
  t_chunk *first_chunk;
  t_free_chunk *first_free_chunk;
  unsigned int active_chunk_count; // checked on freeing to know if heap should
                                   // be unmapped
  size_t size;
} t_heap;

typedef struct s_arenas {
  t_heap *tiny_first;
  t_heap *small_first;
  t_heap *large_first;
  // t_heap *tiny_last;
  // t_heap *small_last;
  // t_heap *large_last;
  enum FUNCTION_CALLED function_called;
} t_arenas;

typedef struct s_mem_usage t_mem_usage;

typedef struct s_mem_usage {
  size_t bytes_mapped;
  size_t bytes_used;
} t_mem_usage;

extern t_arenas g_global;

// MAIN
void ft_free(void *ptr);
void *ft_malloc(size_t size);
// void *realloc(void *ptr, size_t size);
void show_alloc_mem();
void show_alloc_mem_ex();

// SHOW ALLOC
void printHeapTitle(const unsigned int count);
size_t getLargeHeapSize(t_heap *heap);
size_t getHeapSize(t_heap *heap, const enum HEAP_TYPE heap_type);
t_mem_usage printHeaps(t_heap *heap, const enum HEAP_TYPE heap_type,
                       const bool hex);
t_mem_usage addMem(const t_mem_usage a, const t_mem_usage b);

// HEAPS
t_heap *newHeap(const size_t bytesRequested, const enum HEAP_TYPE heap_type);
t_heap **getFirstHeap(const enum HEAP_TYPE heap_type);
void removeHeap(t_heap *heap);

// CHUNKS
t_chunk *allocChunk(const size_t bytesNeeded);

// UTILS
enum HEAP_TYPE getHeapType(const size_t bytesNeeded);
t_heap *getHeapStart(const enum HEAP_TYPE heap_type);
void *getPayloadAddr(t_chunk *chunk);
t_chunk *getHeaderAddr(void *payload);

// ERRORS
void errorHeapMetadataCorruption();
void errorDoubleFree();

// DEBUGS
void debugError(char *str);
void debugInfo(char *str);
void printFreeChunks(const t_heap *heap);
void printHeapChunks(t_heap *heap);

// LIBFT
int ft_strlen(const char *str);
char *ft_strcpy(char *dest, char *src);
void ft_bzero(void *dst, const size_t n);
void ft_putnbr_fd(long n, int fd);
void ft_putsize_t(size_t n, int fd);

// PRINT
void printLine(const char *str);
void printStr(const char *str);
void printAddr(const void *addr, const bool newline);
void printVal(size_t val, char *varName);

#endif

#ifdef __cplusplus
}
#endif
