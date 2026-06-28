/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helauren <helauren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 17:23:46 by helauren          #+#    #+#             */
/*   Updated: 2023/11/27 17:30:28 by helauren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	test(const char *nptr, int *i, long *sign)
{
	if (nptr[*i] == '+')
		(*i)++;
	if (nptr[*i] == '-')
	{
		*sign = -1;
		(*i)++;
	}
}

int	ft_atoi(const char *nptr)
{
	int		i;
	long	n;
	long	sign;
	long	value;

	i = 0;
	sign = 1;
	n = 0;
	while ((nptr[i] >= 9 && nptr[i] <= 13) || nptr[i] == ' ')
		i++;
	if (nptr[i] == '+' && nptr[i + 1] == '-')
		return (0);
	test(nptr, &i, &sign);
	while ((nptr[i] >= '0' && nptr[i] <= '9'))
	{
		value = nptr[i] - '0';
		n = n * 10 + value;
		i++;
	}
	return (sign * n);
}

void	ft_putnbr_fd(int n, int fd)
{
	long int	nb;
	char		towrite;

	nb = (long)n;
	if (nb < 0)
	{
		nb = -nb;
		write(fd, "-", 1);
	}
	if (nb > 9)
	{
		ft_putnbr_fd(nb / 10, fd);
	}
	towrite = nb % 10 + '0';
	write(fd, &towrite, 1);
}

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	if (str == NULL || str[i] == 0)
		return (0);
	while (str[i])
		i++;
	return (i);
}
