/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 21:01:41 by dopereir          #+#    #+#             */
/*   Updated: 2025/02/12 20:53:00 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <pthread.h>
# include <aio.h>
# include <time.h>
# include <stdbool.h>
# include <sys/time.h>

typedef enum e_state
{
	THINKING = 0,
	SLEEPING = 1,
	EATING = 2,
	DEAD = 3,
}			t_status;

typedef struct s_data
{
	int				n_philos;
	int				time_to_die;
	int				time_to_eat;
	long			last_meal_time;
	int				time_to_sleep;
	int				n_of_times_philos_eat;
	long			start_time_ms;
	pthread_t		thread_id;
	pthread_mutex_t	*print_message;
	int				philo_id;
	t_status		status;
}				t_data;

typedef struct s_list
{
	t_data			data;
	pthread_mutex_t	fork;
	struct s_list	*prev;
	struct s_list	*next;
}				t_list;

void	init_main_data(t_data *data, int ac, char **av);
t_data	copy_data(t_data *data);
t_list	*create_node(t_data *data);
void	create_circularll_philos(t_list *head, t_data *data, int n);
//utils.c
int		ft_atoi(const char *str);
long	get_current_time_ms(void);
void	print_message(t_data *data, const char *action);
//routines.c
void	*thread_function(void *arg);

#endif
