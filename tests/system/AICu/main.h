#ifndef MAIN_H
#define MAIN_H

#include "ft_printf/ft_printf.h"
#include "libft/libft.h"
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE_READ_LINE 100000

#define NOONE 0
#define USER 1
#define AI 2

enum {
  win,
  lose,
};

enum {
  ON,
  OFF,
};

typedef struct s_data {
  char **lines;
  int currHeap;
  int turn;
  bool heapEmpty;
  int *winLose;
  int winner;
  int option;
} t_data;

void AIturn(t_data *data, int la);
void userTurn(t_data *data);
char *readLine(int fd, char *stat);
int acIs1(t_data *data);

#endif
