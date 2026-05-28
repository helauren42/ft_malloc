#include "main.h"
#include "utils.h"
#include "init.h"

void	userTurn(t_data *data)
{
	ft_printf("It's your turn, now pick how many you want to remove\n");
	ft_printf("choose between 1 and ");
	int	left = ft_atoi(data->lines[data->currHeap]);
	int	max = left;
	if(left == 1)
		ft_printf("1\n$>: ");
	else if(left == 2)
		ft_printf("2\n$>: ");
	else
	{
		max = 3;
		ft_printf("3\n$>: ");
	}
	char	*userInput = NULL;
	while(1)
	{
		userInput = readLine(STDIN_FILENO, userInput);
		if(ft_strlen(userInput) >= 1)
			userInput[ft_strlen(userInput) -1] = 0;
		if(isValidUserInput(userInput, max) == true)
			break ;
		ft_printf("Try again: ");
		free(userInput);
		userInput = NULL;
	}
	int userPick = ft_atoi(userInput);
	free(userInput);
	free(data->lines[data->currHeap]);
	data->lines[data->currHeap] = ft_itoa(left - userPick);
	data->turn = AI;
	if(ft_atoi(data->lines[data->currHeap]) == 0)
	{
		outLastPiece("YOU");
		data->heapEmpty = true;
		if(data->currHeap == 0)
			data->winner = AI;
	}
}

void	AIturn(t_data *data, int la)
{
	int	move = 0;
	int	currSticks = ft_atoi(data->lines[data->currHeap]);
	if(data->winLose[data->currHeap] == win) // if AI wants to win
	{
		if(is_1_5_9(currSticks) == false) // in favourable position
		{
			if(currSticks % 4 == 0) // 4
				move = 3;
			else if(currSticks % 4 == 2) // 6
				move = 1;
			else if(currSticks % 4 == 3) // 7
				move = 2;
			else
				move = 1;
		}
		else
			move = 1;
	}
	else if(data->winLose[data->currHeap] == lose) // if AI wants to lose
	{
		if(is_4_multi(currSticks) == false) // in favourable position
		{
			if(currSticks % 4 == 1) // 5
				move = 1;
			else if(currSticks % 4 == 2) // 6
				move = 2;
			else if(currSticks % 4 == 3) // 7
				move = 3;
			else 
				move = 1;
		}
		else 
			move = 1;
	}
	data->turn = USER;
	int num = ft_atoi(data->lines[data->currHeap]) -move;
	data->lines[data->currHeap] = updateLine(data->lines[data->currHeap], num);
	if(la == 1)
		outputBoard(data);
	if(data->option == OFF)
		ft_printf("AI has taken off %d pieces\n", move);
	else
		ft_printf("AI has taken %d\n", move);
	if(num == 0 && data->option == OFF)
	{
		data->heapEmpty = true;
		outLastPiece("AI");
		if(data->currHeap == 0)
			data->winner = USER;
	}
}
