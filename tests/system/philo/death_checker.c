/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death_checker.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helauren <helauren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 16:55:00 by helauren          #+#    #+#             */
/*   Updated: 2023/12/01 17:40:34 by helauren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	really_dead(t_death_output *d_o, t_data *data, t_thread_arg **tha)
{
	if (d_o->full < 0)
	{
		output_status(tha[d_o->i]->table->id, " died\n", data, tha[d_o->i]);
		pthread_mutex_lock(data->solo);
		data->dead = -1;
		pthread_mutex_unlock(data->solo);
	}
}

size_t	first_section(t_death_output *d_o, t_data *data, t_thread_arg **tha)
{
	size_t			death_time;

	if (d_o->i == data->philo_n)
		d_o->i = 0;
	pthread_mutex_lock(data->solo);
	death_time = tha[d_o->i]->table->last_meal + data->die_t;
	d_o->full = tha[d_o->i]->table->eaten_meals - tha[d_o->i]->data->meals;
	if (tha[d_o->i]->data->meals < 0)
		d_o->full = -1;
	pthread_mutex_unlock(data->solo);
	return (death_time);
}

void	*death_output(void *arg)
{
	t_thread_arg	**tha;
	t_data			*data;
	size_t			death_time;
	t_death_output	*d_o;

	ft_sleep(2);
	d_o = malloc(sizeof(t_death_output));
	tha = (t_thread_arg **)arg;
	data = tha[0]->data;
	d_o->i = 0;
	pthread_mutex_lock(data->solo);
	d_o->dead2 = data->dead;
	pthread_mutex_unlock(data->solo);
	while (d_o->dead2 == 0)
	{
		death_time = first_section(d_o, data, tha);
		if (get_time() >= death_time)
			really_dead(d_o, data, tha);
		pthread_mutex_lock(data->solo);
		d_o->dead2 = data->dead;
		pthread_mutex_unlock(data->solo);
		d_o->i++;
	}
	free(d_o);
	return (0);
}

int	is_it_full(t_thread_arg **tha, t_data *data, int i)
{
	while (1)
	{
		pthread_mutex_lock(tha[0]->data->solo);
		if ((data->meals >= 0) && (tha[i]->table->eaten_meals >= data->meals))
		{
			pthread_mutex_unlock(tha[0]->data->solo);
			return (1);
		}
		if (data->dead)
		{
			pthread_mutex_unlock(tha[0]->data->solo);
			return (-1);
		}
		pthread_mutex_unlock(tha[0]->data->solo);
	}
	return (0);
}

void	*all_full(void *args)
{
	t_thread_arg	**tha;
	t_data			*data;
	int				i;
	int				is_full;

	tha = (t_thread_arg **)args;
	i = 0;
	data = tha[0]->data;
	while (i < data->philo_n)
	{
		is_full = 0;
		if (is_full == 0)
		{
			is_full = is_it_full(tha, data, i);
			if (is_full == -1)
				return (NULL);
		}
		i++;
	}
	pthread_mutex_lock(data->solo);
	if (i == data->philo_n)
		data->all_full = 1;
	data->dead = -1;
	pthread_mutex_unlock(data->solo);
	return (NULL);
}
