/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helauren <helauren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 17:14:07 by helauren          #+#    #+#             */
/*   Updated: 2023/11/28 21:20:16 by helauren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	checking_input(char **av, int ac)
{
	int	i;
	int	j;

	i = 1;
	if (ac != 5 && ac != 6)
		return (1);
	while (av[i])
	{
		j = 0;
		while (av[i][j])
		{
			if (av[i][j] < '0' || av[i][j] > '9')
				return (1);
			j++;
		}
		i++;
	}
	return (0);
}

char	*first_data(char **av, int ac, t_data *data)
{
	data->dead = 0;
	data->philo_n = ft_atoi(av[1]);
	data->die_t = ft_atoi(av[2]);
	data->eat_t = ft_atoi(av[3]);
	data->sleep_t = ft_atoi(av[4]);
	if (data->philo_n == 0 || data->die_t == 0
		|| data->eat_t == 0 || data->sleep_t == 0)
		return (NULL);
	if (ac == 6)
		data->meals = ft_atoi(av[5]);
	else
		data->meals = -1;
	data->all_full = 0;
	return (av[0]);
}

void	init_side_mutexes(t_data *data)
{
	data->solo = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(data->solo, NULL);
	data->output_status = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(data->output_status, NULL);
	data->take_fork = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(data->take_fork, NULL);
	data->chopsticks = malloc(sizeof(pthread_mutex_t *) * data->philo_n);
}

t_data	*init_data(char **av, int ac, t_data *data)
{
	int		i;

	if (first_data(av, ac, data) == NULL)
	{
		free(data->check_death);
		return (NULL);
	}
	data->check_death = malloc(sizeof(pthread_t));
	data->check_full = malloc(sizeof(pthread_t));
	init_side_mutexes(data);
	i = 0;
	while (i < data->philo_n)
	{
		data->chopsticks[i] = malloc(sizeof(pthread_mutex_t));
		if (pthread_mutex_init(data->chopsticks[i], NULL))
		{
			printf("mutex creation failed\n");
			return (NULL);
		}
		i++;
	}
	data->smartasses = malloc(sizeof(pthread_t) * data->philo_n);
	return (data);
}
