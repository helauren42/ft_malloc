#include <cstddef>
#include <cstdint>
#include <fstream>
#include <ios>
#include <iostream>
#include <iterator>
#include <map>
#include <queue>
#include <sstream>
#include <stdexcept>
#include <utility>

#include "ft_malloc.h"

using namespace std;

class Tester {
private:
  map<uintptr_t, size_t> reachables;
  map<uintptr_t, size_t> unreachables;
  queue<uintptr_t> doubleFrees;

  inline bool rmReachable(const uintptr_t &ptr) {
    for (auto it = this->reachables.rbegin(); it != this->reachables.rend(); it++) {
      if (it->first == ptr) {
        this->reachables.erase(it->first);
        return true;
      }
    }
    return false;
  }
  inline void addUnreachable(const uintptr_t ptr, const size_t &size) {
    this->unreachables[ptr] = size;
  }
  inline void addReachable(const uintptr_t ptr, const size_t &size) {
    this->reachables.insert_or_assign((uintptr_t)ptr, size);
  }

  inline const std::string stringifyQueue(queue<size_t> &q) {
    if (q.empty())
      return "[]";
    stringstream ss;
    ss << "[";
    while (q.size() > 0) {
      ss << "0x" << hex << (q.front() - T_CHUNK_SIZE);
      if (q.size() > 1)
        ss << ", ";
      q.pop();
    }
    ss << "]";
    return ss.str();
  }

  inline const string expectedOutString(const map<uintptr_t, size_t> &allLeaks) {
    queue<size_t> tiny;
    queue<size_t> small;
    queue<size_t> large;
    for (const auto &[ptr, size] : allLeaks) {
      if (size <= TINY_MAX_PAYLOAD)
        tiny.push(ptr);
      else if (size <= SMALL_MAX_PAYLOAD)
        small.push(ptr);
      else
        large.push(ptr);
    }
    string ret = this->stringifyQueue(tiny) + "\n";
    ret += this->stringifyQueue(small) + "\n";
    ret += this->stringifyQueue(large) + "\n";
    cout << "RET: " << ret << endl;
    return ret;
  }
  inline void output_expectations() {
    this->reachables.merge(this->unreachables);
    map<uintptr_t, size_t> allLeaks = this->reachables;
    ofstream file;
    file.open("expected_outputs.txt", ios_base::trunc | ios_base::out);
    if (!file.is_open())
      throw runtime_error("Failed to open file: expected_outputs.txt");
    std::string out = this->expectedOutString(allLeaks);
    file.write(out.c_str(), out.size());
  }

public:
  Tester() {};
  ~Tester() {
    output_expectations();
  };
  void *wrap_malloc(const size_t &size, const uintptr_t ptr) {
    // process old address
    if (ptr) {
      const auto &leaking = this->reachables.find(ptr);
      if (leaking != this->reachables.end())
        addUnreachable(leaking->first, leaking->second);
    }
    // process new address
    const uintptr_t new_ptr = (const uintptr_t)ft_malloc(size);
    addReachable(new_ptr, size);
    return (void *)new_ptr;
  }
  void wrap_free(void *ptr) {
    const uintptr_t uintptr = (const uintptr_t)ptr;
    if (!this->rmReachable(uintptr))
      doubleFrees.push(uintptr);
    ft_free(ptr);
  }
};
