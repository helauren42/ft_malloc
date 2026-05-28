#ifndef UTILS_H
# define UTILS_H

#include "main.h"
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>

bool	is_4_multi(int num);
bool	is_1_5_9(int num);

int		line_isspace(char *s);
int		line_isDigitSpace(char *s);

char*	updateLine(char *line, int num);
void	outLastPiece(char *s);
void	outputBoard(t_data *data);
void	outputWinner(t_data *data);

bool	isValidUserInput(char *userInput, int left);
bool	isValidLine(char *line);

#endif