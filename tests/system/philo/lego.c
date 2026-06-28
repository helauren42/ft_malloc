/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lego.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helauren <helauren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 18:13:47 by helauren          #+#    #+#             */
/*   Updated: 2024/06/24 12:18:54 by helauren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*thread_f(void *arg)
{
	t_thread_arg	*tha;

	tha = (t_thread_arg *)arg;
	while (1)
	{
		if (output_status(tha->table->id, " is thinking\n", tha->data, tha))
			break ;
		pthread_mutex_lock(tha->data->solo);
		if (tha->data->dead)
		{
			pthread_mutex_unlock(tha->data->solo);
			break ;
		}
		pthread_mutex_unlock(tha->data->solo);
		if (taking_forks(tha))
			break ;
		eating(tha);
	}
	return (0);
}

void	actually_launching_threads(t_data *data, t_thread_arg **tha)
{
	data->i = 0;
	while (data->i < data->philo_n)
	{
		tha[data->i]->table->last_meal = get_time();
		pthread_create(&data->smartasses[data->i],
			NULL, &thread_f, tha[data->i]);
		data->i++;
	}
}

void	launch_threads(t_thread_arg **tha)
{
	t_data	*data;
	void	*void_tha;

	void_tha = (void *)tha;
	data = tha[0]->data;
	actually_launching_threads(data, tha);
	pthread_create(data->check_death, NULL, &death_output, void_tha);
	pthread_create(data->check_full, NULL, &all_full, void_tha);
	data->i = 0;
	while (data->i < data->philo_n)
	{
		pthread_join(data->smartasses[data->i], NULL);
		data->i++;
	}
	pthread_mutex_lock(tha[0]->data->solo);
	data->dead = -1;
	pthread_mutex_unlock(tha[0]->data->solo);
	pthread_join(*data->check_death, NULL);
	pthread_join(*data->check_full, NULL);
}

void	philo(t_data *data, t_table *first)
{
	t_table			*curr;
	t_thread_arg	**tha;

	tha = malloc(sizeof(t_thread_arg *) * data->philo_n);
	curr = first;
	data->i = 0;
	while (data->i < data->philo_n)
	{
		tha[data->i] = malloc(sizeof(t_thread_arg));
		tha[data->i]->data = data;
		tha[data->i]->table = curr;
		curr = curr->right_smartass;
		data->i++;
	}
	data->start_time = get_time();
	launch_threads(tha);
	free_tha(tha, data->philo_n);
}

int	main(int ac, char **av)
{
	t_data		*data;
	t_table		*table;

	data = malloc(sizeof(t_data));
	if (checking_input(av, ac) || init_data(av, ac, data) == NULL)
	{
		free (data);
		printf("Invalid Input !!!!!\nCheck the MIN and ");
		printf("MAX values to input\nEX : ./philo 5 800 200 200 7\n");
		return (1);
	}
	table = malloc(sizeof(t_table) * data->philo_n);
	set_the_table(table, data);
	philo(data, table);
	freeing_philo(data, table);
	return (0);
}
