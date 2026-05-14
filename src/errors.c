#include "ft_malloc.h"
#include <stdlib.h>

static void writeStderr(char *str) {
  write(STDERR_FILENO, str, ft_strlen(str));
  write(STDERR_FILENO, "\n", 1);
}

void errorHeapMetadataCorruption() {
  switch (global.function_called) {
  case MALLOC:
    writeStderr("malloc(): heap metadata corrupted");
    break;
  case REALLOC:
    writeStderr("realloc(): heap metadata corrupted");
    break;
  case FREE:
    writeStderr("free(): heap metadata corrupted");
    break;
  }
  abort();
}

void errorDoubleFree() {
  writeStderr("free(): double free detected");
  abort();
}
