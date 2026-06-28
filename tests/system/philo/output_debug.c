/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output_debug.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helauren <helauren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 15:43:24 by helauren          #+#    #+#             */
/*   Updated: 2023/11/28 21:27:43 by helauren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	output_status(int id, char *s, t_data *data, t_thread_arg *tha)
{
	char			*time_s;
	size_t			now;

	(void) tha;
	pthread_mutex_lock(data->output_status);
	now = get_time();
	time_s = ft_ltoa(now - data->start_time);
	pthread_mutex_lock(data->solo);
	if (data->dead)
	{
		free(time_s);
		pthread_mutex_unlock(data->solo);
		pthread_mutex_unlock(data->output_status);
		return (1);
	}
	pthread_mutex_unlock(data->solo);
	write(1, time_s, ft_strlen(time_s));
	write(1, " ", 1);
	ft_putnbr_fd(id, 1);
	write(1, s, ft_strlen(s));
	free(time_s);
	pthread_mutex_unlock(data->output_status);
	return (0);
}

void	mini_out(int id, char *s, t_data *data)
{
	pthread_mutex_lock(data->output_status);
	ft_putnbr_fd(id, 1);
	write(1, s, ft_strlen(s));
	pthread_mutex_unlock(data->output_status);
}

void	output_table(t_table *table, t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_n)
	{
		printf("id = %d, left_chop = %p, right_chop = %p\n",
			table->id, table->left_chopstick, table->right_chopstick);
		table = table->right_smartass;
		i++;
	}
}
