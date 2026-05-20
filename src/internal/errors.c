#include "ft_malloc.h"
#include <stdlib.h>
#include <unistd.h>

static void writeStderr(char *str) {
  write(STDERR_FILENO, "ERROR: ", 7);
  write(STDERR_FILENO, str, ft_strlen(str));
  write(STDERR_FILENO, "\n", 1);
}

void errorHeapMetadataCorruption() {
  switch (g_global.function_called) {
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
  // TODO check flags and don't print if the flag is off
  writeStderr("free(): double free detected");
  // abort(); // original malloc sends an abort signal but the subject here does
  // not allow us too
}
