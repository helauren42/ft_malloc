/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_time.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helauren <helauren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 14:37:28 by helauren          #+#    #+#             */
/*   Updated: 2023/11/27 17:23:35 by helauren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

size_t	get_time(void)
{
	struct timeval	current_time;
	size_t			miliseconds;

	gettimeofday(&current_time, NULL);
	miliseconds = (current_time.tv_sec * 1000) + (current_time.tv_usec / 1000);
	return (miliseconds);
}

void	ft_sleep(size_t time)
{
	size_t	start;
	size_t	now;

	start = get_time();
	now = start;
	while (start + time > now)
		now = get_time();
}

long long int	get_micro_time(void)
{
	struct timeval	current_time;
	long long int	microseconds;

	gettimeofday(&current_time, NULL);
	microseconds = (long long)current_time.tv_sec
		* 1000000LL + (long long)current_time.tv_usec;
	return (microseconds);
}

void	ft_micro_sleep(long long int time)
{
	long long int	start;
	long long int	now;

	start = get_micro_time();
	now = start;
	while (start + time > now)
		now = get_micro_time();
}
