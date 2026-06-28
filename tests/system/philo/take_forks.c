/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   take_forks.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helauren <helauren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 16:53:35 by helauren          #+#    #+#             */
/*   Updated: 2023/12/01 16:37:58 by helauren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	take_forks_even(t_thread_arg *tha)
{
	set_urgency(tha);
	pthread_mutex_lock(tha->data->solo);
	if (tha->data->dead)
	{
		pthread_mutex_unlock(tha->data->solo);
		return (1);
	}
	pthread_mutex_unlock(tha->data->solo);
	pthread_mutex_lock(tha->table->left_chopstick);
	if (output_status(tha->table->id, " has taken a fork\n", tha->data, tha))
	{
		pthread_mutex_unlock(tha->table->left_chopstick);
		return (0);
	}
	pthread_mutex_lock(tha->table->right_chopstick);
	if (output_status(tha->table->id, " has taken a fork\n", tha->data, tha))
	{
		pthread_mutex_unlock(tha->table->right_chopstick);
		pthread_mutex_unlock(tha->table->left_chopstick);
		return (0);
	}
	return (-1);
}

int	take_forks_odd2(t_thread_arg *tha)
{
	if (tha->table->left_chopstick == NULL)
	{
		ft_sleep(tha->data->die_t);
		pthread_mutex_unlock(tha->table->right_chopstick);
		return (1);
	}
	pthread_mutex_lock(tha->table->left_chopstick);
	if (output_status(tha->table->id, " has taken a fork\n", tha->data, tha))
	{
		pthread_mutex_unlock(tha->table->left_chopstick);
		pthread_mutex_unlock(tha->table->right_chopstick);
		return (0);
	}
	return (-1);
}

int	take_forks_odd(t_thread_arg *tha)
{
	int	i;

	if (tha->table->left_chopstick != NULL)
		set_urgency(tha);
	pthread_mutex_lock(tha->data->solo);
	if (tha->data->dead)
	{
		pthread_mutex_unlock(tha->data->solo);
		return (1);
	}
	pthread_mutex_unlock(tha->data->solo);
	pthread_mutex_lock(tha->table->right_chopstick);
	if (output_status(tha->table->id, " has taken a fork\n", tha->data, tha))
	{
		pthread_mutex_unlock(tha->table->right_chopstick);
		return (0);
	}
	i = take_forks_odd2(tha);
	if (i != -1)
		return (i);
	return (-1);
}

int	taking_forks(t_thread_arg *tha)
{
	int	i;

	pthread_mutex_lock(tha->data->solo);
	if (tha->data->dead)
	{
		pthread_mutex_unlock(tha->data->solo);
		return (0);
	}
	pthread_mutex_unlock(tha->data->solo);
	if (tha->table->id % 2 == 0)
	{
		i = take_forks_even(tha);
		if (i != -1)
			return (i);
	}
	else
	{
		i = take_forks_odd(tha);
		if (i != -1)
			return (i);
	}
	return (0);
}
