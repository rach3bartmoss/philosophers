/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 19:45:37 by dopereir          #+#    #+#             */
/*   Updated: 2025/03/16 03:15:50 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <unistd.h>

bool	try_pick_forks(t_list *philo)
{
	t_list	*first_fork;
	t_list	*second_fork;

	if (check_if_simulation_should_stop(philo))
		return (false);
	if (philo->data.philo_id % philo->data.n_philos
		< (philo->data.philo_id + 1) % philo->data.n_philos)
	{
		first_fork = philo;
		second_fork = philo->prev;
	}
	else
	{
		first_fork = philo->prev;
		second_fork = philo;
	}
	if (!helper_pick_forks(philo, first_fork, second_fork))
		return (false);
	return (true);
}

void	release_forks(t_list *philo)
{
	t_list	*first_fork;
	t_list	*second_fork;

	if (philo->data.philo_id % philo->data.n_philos
		< (philo->data.philo_id + 1) % philo->data.n_philos)
	{
		first_fork = philo;
		second_fork = philo->prev;
	}
	else
	{
		first_fork = philo->prev;
		second_fork = philo;
	}
	pthread_mutex_unlock(&second_fork->fork);
	pthread_mutex_unlock(&first_fork->fork);
}

void	philosopher_eat(t_list *philo)
{
	long	eat_start;
	long	remain;

	routine_meal_lock_helper(philo);
	print_message(&philo->data, "is eating");
	eat_start = get_current_time_ms();
	while (get_elapsed_time(eat_start) < philo->data.time_to_eat)
	{
		if (check_if_simulation_should_stop(philo))
			return ;
		remain = philo->data.time_to_eat - get_elapsed_time(eat_start);
		if (remain > 1)
			usleep(900);
	}
	if (philo->data.n_of_times_philos_eat > 0)
	{
		if (helper_philo_eat(philo) != 1)
			return ;
	}
	else if (philo->data.n_of_times_philos_eat == -1)
	{
		if (check_if_simulation_should_stop(philo))
			return ;
		print_message(&philo->data, "is sleeping");
	}
}

void	philosopher_sleep(t_list *philo)
{
	long	sleep_start;
	long	remain;

	sleep_start = get_current_time_ms();
	while (get_elapsed_time(sleep_start) < philo->data.time_to_sleep)
	{
		if (check_if_simulation_should_stop(philo))
			return ;
		remain = philo->data.time_to_sleep - get_elapsed_time(sleep_start);
		if (remain > 1)
			usleep(1000);
	}
}

void	*philosopher_routine(void *arg)
{
	t_list	*philo;

	philo = (t_list *)arg;
	routine_stop_lock_helper(philo);
	if (philo->data.n_philos == 1)
		return (one_philo_handler(philo), NULL);
	if (philo->data.philo_id % 2 == 0)
		usleep((philo->data.time_to_eat * 100) / 2);
	while (!check_if_simulation_should_stop(philo))
	{
		if (try_pick_forks(philo))
		{
			if (!helper_routine_main_iteration(philo))
				return (NULL);
			if (!check_if_simulation_should_stop(philo))
			{
				philosopher_sleep(philo);
				if (!check_if_simulation_should_stop(philo))
					helper_routine_sync(philo);
			}
		}
		else
			usleep(100 + (philo->data.philo_id % 3) * 100);
	}
	return (NULL);
}
