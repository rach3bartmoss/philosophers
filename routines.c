/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 19:45:37 by dopereir          #+#    #+#             */
/*   Updated: 2025/03/01 06:08:34 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	try_pick_forks(t_list *philo)
{
	if (check_if_simulation_should_stop(philo))
		return (false);
	if (philo->data.philo_id % 2 != 0)
		usleep(100);
	if (philo->data.philo_id % 2 == 0)
	{
		if (helper_pick_forks(philo) == false)
			return (false);
	}
	else
	{
		if (helper_pick_forks_rev(philo) == false)
			return (false);
	}
	return (!check_if_simulation_should_stop(philo));
}

void	release_forks(t_list *philo)
{
	pthread_mutex_unlock(&philo->fork);
	pthread_mutex_unlock(&philo->prev->fork);
}

void	philosopher_eat(t_list *philo)
{
	long	eat_start;
	long	remain;

	philo->data.last_meal_time = get_current_time_ms();
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
	philo->data.last_meal_time = get_current_time_ms();
	if (philo->data.n_philos == 1)
		return (one_philo_handler(philo), NULL);
	while (!check_if_simulation_should_stop(philo))
	{
		if (try_pick_forks(philo))
		{
			if (check_if_simulation_should_stop(philo))
				return (release_forks(philo), NULL);
			philosopher_eat(philo);
			release_forks(philo);
			if (philo->data.n_of_times_philos_eat == 0)
				return (NULL);
			if (!check_if_simulation_should_stop(philo))
			{
				philosopher_sleep(philo);
				if (!check_if_simulation_should_stop(philo))
					print_message(&philo->data, "is thinking");
			}
		}
	}
	return (NULL);
}
