/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_the_table.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+
	+:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+
	+#+           */
/*   Created: 2023/11/20 03:15:33 by marvin            #+#    #+#             */
/*   Updated: 2023/11/20 03:15:33 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	link_right_smartass(t_table *first, t_table *curr)
{
	t_table	*prev;

	curr->right_smartass = first;
	prev = curr;
	curr = curr->left_smartass;
	while (curr != first)
	{
		curr->right_smartass = prev;
		prev = curr;
		curr = curr->left_smartass;
	}
	curr->right_smartass = prev;
}

void	set_chopsticks(t_table *first, t_data *data)
{
	t_table	*curr;
	t_table	*left;
	int		i;

	i = 0;
	curr = first;
	while (i < data->philo_n)
	{
		curr->right_chopstick = data->chopsticks[i];
		curr = curr->right_smartass;
		i++;
	}
	i = 0;
	curr = first;
	while (i < data->philo_n)
	{
		left = curr->left_smartass;
		curr->left_chopstick = left->right_chopstick;
		curr = curr->right_smartass;
		i++;
	}
}

void	lonely_philo(t_table *first, t_data *data)
{
	first->id = 1;
	first->the_smartest = data->smartasses[0];
	first->right_chopstick = data->chopsticks[0];
	first->eaten_meals = 0;
	first->left_chopstick = NULL;
	first->left_smartass = NULL;
	first->right_smartass = NULL;
}

t_table	*main_table(t_table *first, t_data *data)
{
	t_table	*curr;
	t_table	*prev;
	int		i;

	i = 0;
	prev = NULL;
	curr = first;
	while (i < data->philo_n)
	{
		if (i > 0)
			curr = malloc(sizeof(t_table));
		curr->id = i + 1;
		curr->full = 0;
		curr->eaten_meals = 0;
		curr->the_smartest = data->smartasses[i];
		if (prev != NULL)
			curr->left_smartass = prev;
		i++;
		prev = curr;
	}
	return (curr);
}

void	set_the_table(t_table *first, t_data *data)
{
	t_table	*curr;

	if (data->philo_n == 1)
	{
		lonely_philo(first, data);
		return ;
	}
	curr = main_table(first, data);
	first->left_smartass = curr;
	link_right_smartass(first, curr);
	set_chopsticks(first, data);
}
