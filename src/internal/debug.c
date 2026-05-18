#include "ft_malloc.h"

inline void debugInfo(char *str) {
  if (!DEV)
    return;
  write(STDERR_FILENO, str, ft_strlen(str));
  write(STDERR_FILENO, "\n", 1);
}

inline void debugError(char *str) {
  if (!DEV)
    return;
  write(STDERR_FILENO, "MALLOC ERROR: ", 14);
  write(STDERR_FILENO, str, ft_strlen(str));
  write(STDERR_FILENO, "\n", 1);
}
