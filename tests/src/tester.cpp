#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <ios>
#include <iostream>
#include <iterator>
#include <map>
#include <queue>
#include <sstream>
#include <stdexcept>
#include <sys/mman.h>
#include <sys/types.h>
#include <utility>
#include <vector>

#include "malloc.h"

using namespace std;

inline constexpr const char *RED = "\033[31m";
inline constexpr const char *GREEN = "\033[32m";
inline constexpr const char *RESET = "\033[0m";

typedef struct s_nest {
  void *anything_else;
  void *something_else;
  char d;
  int a;
} t_nest;

typedef struct s_test {
  t_nest *nest;
  void *anything;
  void *something;
  char c;
  int b;
} t_test;

template <typename T> inline const std::string stringifyVec(vector<T> &ptrs) {
  if (ptrs.empty())
    return "[]";
  stringstream ss;
  ss << "[";
  size_t i = 0;
  size_t last = ptrs.size();
  for (size_t i = 0; i < ptrs.size(); i++) {
    ss << "0x" << hex << (ptrs[i]);
    if (i < ptrs.size() - 1)
      ss << ", ";
  }
  ss << "]";
  return ss.str();
}

inline const std::string stringifyMap(map<uintptr_t, size_t> &dict) {
  if (dict.empty())
    return "{}";
  stringstream ss;
  ss << "{";
  size_t i = 0;
  size_t last = dict.size();
  for (auto it = dict.begin(); it != dict.end(); ++it) {
    ss << "0x" << std::hex << it->first;
    if (std::next(it) != dict.end())
      ss << ", ";
  }
  ss << "}";
  return ss.str();
}

class Validator {
private:
  inline void printDiff(vector<uintptr_t> &expecting, vector<uintptr_t> &outcome) {
    cout << RED << "  Expected: " << stringifyVec(expecting) << endl;
    cout << RED << "  Received: " << stringifyVec(outcome) << endl;
    vector<uintptr_t> minus;
    set_difference(expecting.begin(), expecting.end(), outcome.begin(), outcome.end(), back_inserter(minus));
    if (!minus.size())
      return;
    cout << RED << "  -Diff: " << endl << "    ";
    for (size_t i = 0; i < minus.size(); i++) {
      cout << hex << minus[i];
      if (i < minus.size() - 1)
        cout << ", ";
    }
    cout << RESET << endl;
    vector<uintptr_t> plus;
    set_difference(outcome.begin(), outcome.end(), expecting.begin(), expecting.end(), back_inserter(plus));
    if (!plus.size())
      return;
    cout << RED << "  +Diff: " << endl << "    ";
    for (size_t i = 0; i < plus.size(); i++) {
      cout << hex << plus[i];
      if (i < plus.size() - 1)
        cout << ", ";
    }
    cout << RESET << endl;
  }

  inline void heap_validation_error_title(bool &status) {
    if (!status)
      return;
    cerr << RED << "  Error with arena" << RESET << endl;
    status = false;
  }
  inline void getArena(vector<uintptr_t> &vec, t_heap *c_heap) {
    while (c_heap) {
      t_chunk *chunk = c_heap->first_chunk;
      size_t activeChunks = 0;
      while (chunk) {
        if (!chunk->is_free) {
          vec.push_back((uintptr_t)chunk);
          activeChunks++;
        }
        chunk = chunk->next;
      }
      c_heap = c_heap->next;
    }
    sort(vec.begin(), vec.end());
  }
  inline void validate_arena(vector<uintptr_t> &expecting, vector<uintptr_t> &outcome) {
    bool status = true;
    if (expecting.size() != outcome.size()) {
      heap_validation_error_title(status);
      printDiff(expecting, outcome);
      return;
    }
    size_t len = expecting.size();
    for (size_t i = 0; i < len; i++) {
      if (expecting[i] != outcome[i]) {
        heap_validation_error_title(status);
        printDiff(expecting, outcome);
        return;
      }
    }
    cout << GREEN << "  Success" << RESET << endl;
  }

public:
  inline void validate_result(vector<uintptr_t> &tiny, vector<uintptr_t> &small, vector<uintptr_t> &large) {
    sort(tiny.begin(), tiny.end());
    sort(small.begin(), small.end());
    sort(large.begin(), large.end());
    vector<uintptr_t> tiny_arena;
    vector<uintptr_t> small_arena;
    vector<uintptr_t> large_arena;
    getArena(tiny_arena, g_global.tiny_first);
    getArena(small_arena, g_global.small_first);
    getArena(large_arena, g_global.large_first);
    cout << "Tiny: " << endl;
    validate_arena(tiny, tiny_arena);
    cout << "Small: " << endl;
    validate_arena(small, small_arena);
    cout << "Large: " << endl;
    validate_arena(large, large_arena);
  }
};

class Tester : Validator {
private:
  map<uintptr_t, size_t> reachables;
  map<uintptr_t, size_t> unreachables;
  queue<uintptr_t> doubleFrees;
  vector<uintptr_t> tiny;
  vector<uintptr_t> small;
  vector<uintptr_t> large;
  bool throwing;

  inline bool rmReachable(const uintptr_t &ptr) {
    cout << "Erasing: " << hex << (uintptr_t)ptr - T_CHUNK_SIZE << endl;
    for (auto it = reachables.rbegin(); it != reachables.rend(); it++) {
      if (it->first == ptr) {
        reachables.erase(it->first);
        return true;
      }
    }
    return false;
  }
  inline void addUnreachable(const uintptr_t ptr, const size_t &size) { unreachables[ptr] = size; }
  inline void addReachable(const uintptr_t ptr, const size_t &size) {
    cout << "Adding: " << hex << (uintptr_t)ptr - T_CHUNK_SIZE << endl;
    reachables.insert_or_assign((uintptr_t)ptr, size);
  }

  inline void initExpectedArenas() {
    reachables.merge(unreachables);
    map<uintptr_t, size_t> allLeaks = reachables;
    for (const auto &[ptr, size] : allLeaks) {
      if (size <= TINY_MAX_PAYLOAD)
        tiny.push_back(ptr - T_CHUNK_SIZE);
      else if (size <= SMALL_MAX_PAYLOAD)
        small.push_back(ptr - T_CHUNK_SIZE);
      else
        large.push_back(ptr - T_CHUNK_SIZE);
    }
  }
  inline void clearHeap(t_heap *heap) {
    while (heap) {
      t_heap *curr = heap;
      heap = heap->next;
      munmap(curr, curr->size);
    }
  }
  inline void clearHeaps() {
    clearHeap(g_global.tiny_first);
    g_global.tiny_first = NULL;
    clearHeap(g_global.small_first);
    g_global.small_first = NULL;
    clearHeap(g_global.large_first);
    g_global.large_first = NULL;
  }

public:
  Tester() { throwing = true; };
  Tester(bool throwing) { this->throwing = throwing; };
  ~Tester() {
    initExpectedArenas();
    validate_result(tiny, small, large);
    clearHeaps();
  };

  void *wrap_malloc(const size_t size, const void *ptr_void) {
    uintptr_t ptr = (uintptr_t)ptr_void;
    // process old address
    if (ptr) {
      const auto &leaking = reachables.find(ptr);
      if (leaking != reachables.end())
        addUnreachable(leaking->first, leaking->second);
      rmReachable(ptr);
    }
    // process new address
    const uintptr_t new_ptr = (const uintptr_t)ft_malloc(size);
    if (this->throwing && !new_ptr)
      throw runtime_error("malloc failed to alloc memory");
    addReachable(new_ptr, size);
    return (void *)new_ptr;
  }

  void wrap_free(void *ptr) {
    const uintptr_t uintptr = (const uintptr_t)ptr;
    if (!rmReachable(uintptr)) {
      doubleFrees.push(uintptr);
    }
    ft_free(ptr);
  }
  void *wrap_realloc(size_t size, void *ptr_void) {
    void *new_ptr = ft_realloc(ptr_void, size);
    rmReachable((uintptr_t)ptr_void);
    addReachable((uintptr_t)new_ptr, size);
    return new_ptr;
  }
};
