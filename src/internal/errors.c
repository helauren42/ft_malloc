#include "ft_malloc.h"
#include <stdlib.h>
#include <unistd.h>

static void writeStderr(char *str) {
  write(STDERR_FILENO, str, ft_strlen(str));
  write(STDERR_FILENO, "\n", 1);
}

void errorHeapMetadataCorruption() {
  switch (g_heaps.function_called) {
  case MALLOC:
    writeStderr("malloc(): heap metadata corrupted");
    break;
  case REALLOC:
    writeStderr("realloc(): heap metadata corrupted");
    break;
  case FREE:
    writeStderr("free(): heap metadata corrupted");
    break;
  default:
    debugError("errorHeapMetadataCorruption default case");
  }
  abort();
}

void errorDoubleFree() {
  writeStderr("free(): double free detected");
  abort();
}
