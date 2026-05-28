#include "main.h"
#include "utils.h"

bool	findEnd(char *line)
{
	int	i = 0;
	while(ft_isspace(line[i]) == 1)
		i++;
	if(line[i] == 0 || line[i] != 'E')
		return(false);
	if(ft_strcmp("END", line) == 0)
	{
		free(line);
		return(true);
	}
	if(ft_strcmp("END", line) )
	free(line);
	line = NULL;
	return (false);
}

bool	findRedo(char *line)
{
	char *preLine;
	int	i = 0;
	while(ft_isspace(line[i]) == 1)
		i++;
	if(line[i] == 0 || line[i] != 'R')
		return(false);
	preLine = line;
	line = ft_strtrim(line, " \n");
	free(preLine);
	if(ft_strcmp("REDO", line) == 0)
	{
		free(line);
		return(true);
	}
	free(line);
	line = NULL;
	return (false);
}

char	**addTo2dStr(char **str, char *line)
{
	if(str == NULL)
	{
		str = malloc(sizeof(char *) * 2);
		str[0] = line;
		str[1] = NULL;
		return (str);
	}
	char	**ret = malloc(sizeof(char *) * (ft_strlen2d(str) +2));
	int i = 0;
	while(str[i])
	{
		ret[i] = str[i];
		i++;
	}
	ret[i] = line;
	ret[++i] = NULL;
	free(str);
	return (ret);
}

int	findProtection(char *line)
{
	int	i = 0;
	while (line[i])
	{
		if(line[i] == 'E' || line[i] == 'R')
		{
			if(ft_strcmp("END", &line[i]) && ft_strcmp("REDO", &line[i]))
				return (1);
		}
		i++;
	}
	return (0);
}

int	acIs1(t_data *data)
{
	char	*line = NULL;
	bool	foundEnd = false;
	char	*preLine = NULL;

	ft_printf("To type in the values for every individual heap, type digits and press enter to submit heap value, and type \"END\" once everything has been input\n");
	ft_printf("If you made a typo you can type \"REDO\" to start over\n");
	while(1)
	{
		ft_printf("$>: ");
		line = readLine(STDIN_FILENO, line);
		preLine = line;
		line = ft_strtrim(line, " ");
		free(preLine);
		if(findProtection(line) == 1)
		{
			ft_printf("ERROR: Invalid input\n");
			free(line);
			line = NULL;
			continue ;
		}
		foundEnd = findEnd(line);
		if(foundEnd == true)
		{
			if(ft_strlen2d(data->lines) == 0)
			{
				line = NULL;
				ft_printf("\nError: Can't end now, the board is empty\nWhat is this mentality ?! FINISH WHAT YOU START!\n");
				continue;
			}
			break ;
		}
		if(findRedo(line) == true)
		{
			if(ft_strlen2d(data->lines) == 0)
				ft_printf("\nYou typed REDO when the board is already empty, useless but okay..\n");
			free_double_str(data->lines);
			data->lines = NULL;
			line = NULL;
			continue ;
		}
		char *toFree = line;
		line = ft_strtrim(line, " \n");
		if(toFree != NULL)
			free(toFree);
		if(line == NULL || (line_isDigitSpace(line) || isValidLine(line) == false))
		{
			ft_printf("ERROR: Invalid input\n");
			free(line);
			line = NULL;
			continue;
		}
		data->lines = addTo2dStr(data->lines, line);
		line = NULL;
	}
	return (0);
}
