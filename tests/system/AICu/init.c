#include "init.h"
#include "utils.h"

char	*readFile(int fd)
{
	int	br = 1;
	char	buff[10];
	char	*all = NULL;

	while(br > 0)
	{
		br = read(fd, buff, 9);
		buff[br] = 0;
		all = ft_strjoin(all, buff);
	}
	return (all);
}

int	validFileContent(char **lines)
{
	for (int i = 0; lines[i]; i++)
	{
		if(ft_strlen2d(lines) <= 0)
		{
			ft_printf("Error: file is empty\n");
			return (1);
		}
		if(line_isDigitSpace(lines[i]))
		{
			ft_printf("Error: line has unvalid characters: %s\n", lines[i]);
			return (1);
		}
		if(isValidLine(lines[i]) == false)
			return (1);
	}
	for (int i = 0; lines[i]; i++)
	{
		for (int j = 0; lines[i][j] && lines[i][j] != '\n'; j++) // check that it has at least one digit in the file
			if(ft_isdigit(lines[i][j]) == 1)
				return (0);
		ft_printf("error2\n");
		return (1);
	}
	return (0);
}

void	setWinLose(t_data *data)
{
	data->winLose = malloc(sizeof(int *) * ft_strlen2d(data->lines));
	for(int i = 0; data->lines[i]; i++)
	{
		if(i == 0)
		{
			data->winLose[i] = win;
			continue;
		}
		int prevSticks = ft_atoi(data->lines[i -1]);

		// win next round
		if(data->winLose[i -1] == win && is_4_multi(prevSticks) == true)
			data->winLose[i] = lose;
		else if(data->winLose[i -1] == win)
			data->winLose[i] = win;
		
		// lose next round
		if(data->winLose[i -1] == lose && is_4_multi(prevSticks) == true)
			data->winLose[i] = win;
		else if(data->winLose[i -1] == lose)
			data->winLose[i] = lose;
	}
}

// if you want to win you need your opponent on 1-5-9 and yourself on multiple of 4 or anything else
// if you want to lose you need to put your opponent on a mutliple of 4 and yourself on anything else

void	clear_out_empty_lines(t_data *data)
{
	int count = 0;
	int i = 0;
	char**	toFree = data->lines;
	while(data->lines[i])
	{
		if(line_isspace(data->lines[i]) == 0)
			count++;
		i++;
	}
	int	len = ft_strlen2d(data->lines);
	char **newLines = malloc(sizeof(char *) * (len - count +1));
	int	j = 0;
	for(int i = 0; data->lines[i]; i++)
	{
		if(line_isspace(data->lines[i]) == 1)
		{
			newLines[j] = data->lines[i];
			j++;
		}
		else
			free(data->lines[i]);
	}
	free(toFree);
	newLines[j] = NULL;
	data->lines = newLines;
}
