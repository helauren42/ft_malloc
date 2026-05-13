#include "ft_malloc.h"

inline void writeError(char *str) {
  write(STDERR_FILENO, "MALLOC ERROR: ", 14);
  write(STDERR_FILENO, str, ft_strlen(str));
  write(STDERR_FILENO, "\n", 1);
}
