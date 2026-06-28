/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helauren <helauren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 20:18:17 by helauren          #+#    #+#             */
/*   Updated: 2023/11/29 18:27:41 by helauren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	eating_mid(t_thread_arg *tha)
{
	ft_sleep(tha->data->eat_t / 2);
	pthread_mutex_lock(tha->data->solo);
	tha->table->eaten_meals++;
	pthread_mutex_unlock(tha->data->solo);
	ft_sleep(tha->data->eat_t / 2);
	if (tha->table->id % 2 == 0)
	{
		pthread_mutex_unlock(tha->table->right_chopstick);
		pthread_mutex_unlock(tha->table->left_chopstick);
	}
	else
	{
		pthread_mutex_unlock(tha->table->left_chopstick);
		pthread_mutex_unlock(tha->table->right_chopstick);
	}
	pthread_mutex_lock(tha->data->solo);
}

void	eating(t_thread_arg *tha)
{
	pthread_mutex_lock(tha->data->solo);
	tha->table->last_meal = get_time();
	pthread_mutex_unlock(tha->data->solo);
	if (output_status(tha->table->id, " is eating\n", tha->data, tha))
		return ;
	eating_mid(tha);
	if ((tha->data->dead))
	{
		pthread_mutex_unlock(tha->data->solo);
		return ;
	}
	pthread_mutex_unlock(tha->data->solo);
	ft_micro_sleep(50);
	if (output_status(tha->table->id, " is sleeping\n", tha->data, tha))
		return ;
	ft_sleep(tha->data->sleep_t);
}

void	urgency_even(t_thread_arg *tha)
{
	size_t	fast;

	pthread_mutex_lock(tha->data->solo);
	if (tha->data->dead)
	{
		pthread_mutex_unlock(tha->data->solo);
		return ;
	}
	pthread_mutex_unlock(tha->data->solo);
	pthread_mutex_lock(tha->table->left_chopstick);
	pthread_mutex_lock(tha->table->right_chopstick);
	fast = get_time() - tha->table->last_meal;
	pthread_mutex_lock(tha->data->solo);
	if (fast + tha->data->eat_t >= tha->data->die_t * 2 -10)
		tha->table->urgency = 1;
	if (fast + tha->data->eat_t >= tha->data->die_t -10)
		tha->table->urgency = 2;
	pthread_mutex_unlock(tha->data->solo); 
	pthread_mutex_unlock(tha->table->right_chopstick);
	pthread_mutex_unlock(tha->table->left_chopstick);
}

void	urgency_odd(t_thread_arg *tha)
{
	size_t	fast;

	pthread_mutex_lock(tha->data->solo);
	if (tha->data->dead)
	{
		pthread_mutex_unlock(tha->data->solo);
		return ;
	}
	pthread_mutex_unlock(tha->data->solo);
	pthread_mutex_lock(tha->table->right_chopstick);
	pthread_mutex_lock(tha->table->left_chopstick);
	fast = get_time() - tha->table->last_meal;
	pthread_mutex_lock(tha->data->solo);
	if (fast + tha->data->eat_t >= tha->data->die_t * 2 -10)
		tha->table->urgency = 1;
	if (fast + tha->data->eat_t >= tha->data->die_t -10)
		tha->table->urgency = 2;
	pthread_mutex_unlock(tha->data->solo);
	pthread_mutex_unlock(tha->table->right_chopstick);
	pthread_mutex_unlock(tha->table->left_chopstick);
}

void	set_urgency(t_thread_arg *tha)
{
	ft_micro_sleep(100);
	tha->table->urgency = 0;
	if (tha->table->id % 2 == 0)
		urgency_even(tha);
	else
		urgency_odd(tha);
	if (tha->table->urgency == 0)
		ft_micro_sleep(130);
	if (tha->table->urgency == 1)
		ft_micro_sleep(80);
}
