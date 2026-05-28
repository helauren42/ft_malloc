#ifndef INIT_H
# define INIT_H

#include "main.h"

char	*readFile(int fd);
int		validFileContent(char **lines);
void	setWinLose(t_data *data);
void	clear_out_empty_lines(t_data *data);

#endif