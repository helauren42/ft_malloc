#include "main.h"
#include "utils.h"
#include "init.h"

void	free_everything(t_data *data)
{
	if(data->winLose != NULL)
		free(data->winLose);
	free_double_str(data->lines);
	if(data)
		free(data);
}

void	handleRound(t_data* data)
{
	data->heapEmpty = false;
	while(data->heapEmpty == false)
	{
		if(data->turn == USER)
		{
			outputBoard(data);
			if(data->heapEmpty == false)
				userTurn(data);
			if(data->heapEmpty == false)
				AIturn(data, 0);
			continue;
		}
		else if(data->turn == AI && data->heapEmpty == false)
		{
			if(data->heapEmpty == false)
				AIturn(data, 1);
			if(data->heapEmpty == false)
				userTurn(data);
		}
	}
}

int	main(int ac, char **av)
{
	t_data *data = NULL;
	if(ac > 2)
	{
		if(ac == 3 && ft_strcmp(av[1], "-v") == 0 && line_isDigitSpace(av[2]) == 0)
		{
			data = malloc(sizeof(t_data));
			data->option = ON;
			data->winLose = NULL;
			data->lines = ft_split(av[2], ' ');
		}
		else
		{
			ft_printf("Error:\nWrong number of arguments\n");
			return (1);
		}
	}
	if(ac == 2)
	{
		int fd = open(av[1], O_RDONLY);
		if(fd < 0 || (ft_strlen(av[1]) <= 2 && (ft_strcmp(av[1], ".") == 0 || ft_strcmp(av[1], "/") == 0 || ft_strcmp(av[1], "./") == 0 || ft_strcmp(av[1], "..") == 0)))
		{
			ft_printf("Error:\nCould not open file, make sure it's in folder testFiles\n");
			return (1);
		}
		data = malloc(sizeof(t_data));
		data->option = OFF;
		data->winLose = NULL;
		char *rf = readFile(fd);
		data->lines = ft_split(rf, '\n');
		clear_out_empty_lines(data);
		free(rf);
		close(fd);
	}
	if(ac == 1)
	{
		data = malloc(sizeof(t_data));
		data->option = OFF;
		data->winLose = NULL;
		data->lines = NULL;
		acIs1(data);
	}
	if(validFileContent(data->lines) != 0)
	{
		ft_printf("Error:\nFile content throws an error\n");
		free_everything(data);
		return (1);
	}
	clear_out_empty_lines(data); // needed for option == ON
	setWinLose(data);
	data->turn = AI;
	data->currHeap = ft_strlen2d(data->lines) -1;
	if(data->option == ON)
	{
		AIturn(data, 0);
		free_everything(data);
		return (0);
	}
	data->option = OFF;
	outputBoard(data);
	while(data->currHeap >= 0)
	{
		handleRound(data);
		data->currHeap--;
	}
	outputWinner(data);
	free_everything(data);
	return (0);
}

// int	main(int ac, char **av)
// {
// 	t_data *data;
// 	if(ac > 2)
// 	{
// 		ft_printf("Error:\nWrong number of arguments\n");
// 		return (1);
// 	}
// 	if(ac == 2) // reading from file
// 	{
// 		int fd = open(av[1], O_RDONLY);
// 		if(fd < 0)
// 		{
// 			ft_printf("Error:\nCould not open file, make sure it's in folder testFiles\n");
// 			return (1);
// 		}
// 		data = malloc(sizeof(t_data));
// 		char *rf = readFile(fd);
// 		data->lines = ft_split(rf, '\n');
// 		free(rf);
// 	}
// 	else
// 	{
// 		data = malloc(sizeof(t_data));
// 	}
// 	data->winLose = NULL;
// 	if(validFileContent(data->lines) != 0)
// 	{
// 		ft_printf("Error:\nFile content throws an error\n");
// 		free_everything(data);
// 		return (1);
// 	}
// 	clear_out_empty_lines(data);
// 	setWinLose(data);
// 	data->turn = USER;
// 	data->currHeap = ft_strlen2d(data->lines) -1;
// 	while(data->currHeap >= 0)
// 	{
// 		handleRound(data);
// 		data->currHeap--;
// 	}
// 	outputWinner(data);
// 	free_everything(data);
// 	return (0);
// }
