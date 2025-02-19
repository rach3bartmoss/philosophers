/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 21:01:41 by dopereir          #+#    #+#             */
/*   Updated: 2025/02/19 22:55:16 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <pthread.h>
# include <string.h>
# include <aio.h>
# include <time.h>
# include <stdbool.h>
# include <sys/time.h>

typedef struct s_data
{
	int				n_philos;
	long			time_to_die;
	long			time_to_eat;
	long			last_meal_time;
	long			time_to_sleep;
	int				n_of_times_philos_eat;
	long			start_time_ms;
	pthread_t		thread_id;
	pthread_mutex_t	*print_message;
	int				philo_id;
	bool			*simulation_stop;
	pthread_mutex_t	*stop_mutex;
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
bool	init_philos_threads(t_list *head, int n_philos);
//utils.c
int		ft_atoi(const char *str);
long	get_current_time_ms(void);
void	print_message(t_data *data, const char *action);
long	get_elapsed_time(long start_time_ms);
//routines.c
bool	try_pick_forks(t_list *philo);
void	release_forks(t_list *philo);
void	philosopher_eat(t_list *philo);
void	*philosopher_routine(void *arg);
//clean_up.c
void	cleanup_node(t_list *node);
void	cleanup_circular_list(t_list *head);
void	cleanup_threads(t_list *head, int n_philos);
void	cleanup_all(t_list *head, pthread_mutex_t *print_message,
			t_data *main_data);
//utils2.c
bool	check_if_simulation_should_stop(t_list *philo);
void	stop_simulation(t_list *philo);
bool	check_and_handle_death(t_list *philo);
//routine_helper.c
void	one_philo_handler(t_list *philo);
//monitor.c
void	*monitor_routine(void *arg);

#endif
