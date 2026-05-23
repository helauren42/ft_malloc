#include "ft_malloc.h"
#include <unistd.h>

inline void debugInfo(char *str) {
  if (!DEV)
    return;
  write(STDOUT_FILENO, str, ft_strlen(str));
  write(STDOUT_FILENO, "\n", 1);
}

inline void debugError(char *str) {
  if (!DEV)
    return;
  write(STDERR_FILENO, "MALLOC ERROR: ", 14);
  write(STDERR_FILENO, str, ft_strlen(str));
  write(STDERR_FILENO, "\n", 1);
}
