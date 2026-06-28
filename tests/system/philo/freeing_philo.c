/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freeing_philo.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helauren <helauren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 20:05:14 by helauren          #+#    #+#             */
/*   Updated: 2023/12/01 18:26:01 by helauren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	empty_the_table(t_table *first, t_data *data)
{
	int		i;
	t_table	*last;
	t_table	*curr;

	i = 0;
	last = first;
	while (i < data->philo_n)
	{
		if (i < data->philo_n -1)
			last = last->right_smartass;
		i++;
	}
	last->right_smartass = NULL;
	while (first != NULL)
	{
		curr = first;
		first = first->right_smartass;
		free(curr);
	}
}

void	freeing_philo(t_data *data, t_table *first)
{
	int		i;

	i = 0;
	empty_the_table(first, data);
	while (i < data->philo_n)
	{
		free(data->chopsticks[i]);
		i++;
	}
	free(data->chopsticks);
	free(data->check_death);
	free(data->check_full);
	free(data->solo);
	free(data->take_fork);
	free(data->output_status);
	free(data->smartasses);
	free(data);
}

void	free_tha(t_thread_arg **tha, int max)
{
	int	i;

	i = 0;
	while (i < max)
	{
		free(tha[i]);
		i++;
	}
	free(tha);
}
