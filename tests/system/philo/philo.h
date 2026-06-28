/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helauren <helauren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 17:14:04 by helauren          #+#    #+#             */
/*   Updated: 2023/11/28 21:20:49 by helauren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_table
{
	int				urgency;
	int				id;
	pthread_t		the_smartest;
	struct s_table	*left_smartass;
	struct s_table	*right_smartass;
	pthread_mutex_t	*left_chopstick;
	pthread_mutex_t	*right_chopstick;
	struct timeval	*tv;
	struct timezone	*tz;
	long			sec;
	long			microsec;
	int				full;
	size_t			last_meal;
	int				eaten_meals;
}					t_table;

typedef struct s_data
{
	int				dead;
	size_t			start_time;
	int				i;
	int				philo_n;
	size_t			die_t;
	size_t			eat_t;
	size_t			sleep_t;
	int				meals;
	int				all_full;
	pthread_mutex_t	*solo;
	pthread_mutex_t	*output_status;
	pthread_mutex_t	*take_fork;
	pthread_mutex_t	**chopsticks;
	pthread_t		*check_death;
	pthread_t		*check_full;
	pthread_t		*smartasses;
}					t_data;

typedef struct s_thread_arg
{
	t_data			*data;
	t_table			*table;
}					t_thread_arg;

typedef struct s_death_output
{
	int				i;
	int				full;
	int				dead2;
}					t_death_output;

// init
t_data				*init_data(char **av, int ac, t_data *data);
int					checking_input(char **av, int ac);
void				set_the_table(t_table *first, t_data *data);

// time
size_t				get_time(void);
void				ft_sleep(size_t time);
long long int		get_micro_time(void);
void				ft_micro_sleep(long long int time);
void				set_urgency(t_thread_arg *tha);

// actions
int					taking_forks(t_thread_arg *tha);
void				eating(t_thread_arg *tha);
void				*death_output(void *arg);
void				*all_full(void *args);

// freeing
void				freeing_philo(t_data *data, t_table *first);
void				free_tha(t_thread_arg **tha, int max);

// output
int					output_status(int id, char *s, t_data *data,
						t_thread_arg *tha);

// debugging
void				output_table(t_table *table, t_data *data);
void				mini_out(int id, char *s, t_data *data);

// utils
void				ft_putnbr_fd(int n, int fd);
int					ft_atoi(const char *nptr);
char				*ft_ltoa(long long int n);
int					ft_strlen(char *str);

#endif
