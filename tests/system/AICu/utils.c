#include "utils.h"

bool	isValidLine(char *line)
{
	if(line == NULL || ft_strlen(line) == 0)
	{
		ft_printf("\nYou thought we didn't handle empty newlines or eof?\?!\nPFF lol\n");
		return (false);
	}
	if(line_isDigitSpace(line) == 0)
	{
		int	i = 0;
		while(ft_isdigit(line[i]) == 0) // while it is not digit
			i++;
		while(ft_isdigit(line[i]) == 1) // while it is digit
			i++;
		while(line[i]) // if input is format "5 5"
		{
			if(ft_isdigit(line[i]) == 1)
			{
				ft_printf("ERROR: well tried, tricky input with format \"digit space digit\"\n");
				return (false);
			}
			i++;
		}
	}
	return (true);
}

bool	isValidUserInput(char *userInput, int left)
{
	if(ft_strlen(userInput) == 0)
	{
		ft_printf("\nYou thought we didn't handle empty newlines or eof?\?!\nPFF lol\n");
		return (false);
	}
	ft_printf("left : %d\n, userInputNum : %d\n", left, ft_atoi(userInput));
	if(ft_atoi(userInput) == 0 || ft_atoi(userInput) > left)
	{
		ft_printf("IS FALTY\n");
		ft_printf("WHAT KIND OF INPUT IS THAT?\nI said choose between 1 and ");
		if(left == 1)
			ft_printf("1\n");
		else if(left == 2)
			ft_printf("2\n");
		else
			ft_printf("3\n");
		return (false);
	}
	if(line_isDigitSpace(userInput))
	{
		int	i = 0;
		while(ft_isdigit(userInput[i]) == 0) // while it is not digit
			i++;
		while(ft_isdigit(userInput[i]) == 1) // while it is digit
			i++;
		while(userInput[i]) // if input is format "5 5"
		{
			if(ft_isdigit(userInput[i]) == 1)
			{
				ft_printf("ERROR: well tried, tricky input with format \"digit space digit\"\n");
				return (false);
			}
			i++;
		}
		ft_printf("ERROR: Invalid input only spaces and digits allowed\n");
		return (false);
	}
	return (true);
}

bool	is_4_multi(int num)
{
	if(num <= 0)
		return(false);
	num--;
	if(num % 4 == 0)
		return (true);
	return (false);
}

bool	is_1_5_9(int num)
{
	if(num <= 0)
		return(false);
	num--;
	if(num % 4 == 0)
		return (true);
	return (false);
}

int	line_isspace(char *s)
{
	int	i = 0;
	while(s[i] && ft_isspace(s[i]))
		i++;
	if(s[i] == 0)
		return(0);
	return(1);
}

int	line_isDigitSpace(char *s)
{
	int	i = 0;
	while(s[i] && (ft_isspace(s[i]) == 1 || ft_isdigit(s[i]) == 1))
		i++;
	if(s[i] == 0)
		return(0);
	return(1);
}

char*	updateLine(char *line, int num)
{
	free(line);
	line = ft_itoa(num);
	return (line);
}

void	outLastPiece(char *s)
{
	ft_printf("\nTHIS HEAP IS EMPTY!\n%s took the last piece\n\n", s);
}

void	outputBoard(t_data *data)
{
	ft_printf("\nBOARD:\n");
	for(int i = 0; data->lines[i]; i++)
	{
		int sticks = ft_atoi(data->lines[i]);
		if(sticks == 0)
			break ;
		ft_printf("%d   ", sticks);
		while(sticks > 0)
		{
			ft_printf("|");
			sticks--;
		}
		ft_printf("\n");
	}
}

void	outputWinner(t_data *data)
{
	ft_printf("CONGRATULATIONS TO THE WINNER !!!\n");
	if(data->winner == USER)
		ft_printf("Against all odds the user has won\nThis has been a David and Goliath type of fight\n");
	if(data->winner == AI)
		ft_printf("As expected the AI has won, humans seem powerless when facing AI\n\n\n");
}