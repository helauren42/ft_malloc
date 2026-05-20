#include "ft_malloc.h"

inline static void logFree(void *headerAddr) {
  static const char *GREEN = "\033[32m";
  static const char *RESET = "\033[0m";
  printStr(GREEN);
  printStr("Free: ");
  printAddr(headerAddr, true);
  printStr(RESET);
}

inline static void logAlloc(void *headerAddr) {
  static const char *BLUE = "\033[34m";
  static const char *RESET = "\033[0m";
  printStr(BLUE);
  printStr("Malloc: ");
  printAddr(headerAddr, true);
  printStr(RESET);
}
