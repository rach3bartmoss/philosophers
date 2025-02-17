/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 19:45:37 by dopereir          #+#    #+#             */
/*   Updated: 2025/02/17 00:57:50 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	try_pick_forks(t_list *philo)
{
	if (check_if_simulation_should_stop(philo))
		return (false);
	if (philo->data.philo_id % 2 == 0)
	{
		pthread_mutex_lock(&philo->fork);
		print_message(&philo->data, "has taken a fork");
		pthread_mutex_lock(&philo->prev->fork);
		print_message(&philo->data, "has taken a fork");
	}
	else
	{
		usleep(100);
		pthread_mutex_lock(&philo->prev->fork);
		print_message(&philo->data, "has taken a fork");
		pthread_mutex_lock(&philo->fork);
		print_message(&philo->data, "has taken a fork");
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
	print_message(&philo->data, "is eating");
	philo->data.last_meal_time = get_current_time_ms();
	usleep(philo->data.time_to_eat * 1000);
}

void	*philosopher_routine(void *arg)
{
	t_list	*philo;

	philo = (t_list *)arg;
	philo->data.last_meal_time = get_current_time_ms();
	if (philo->data.n_philos == 1)
	{
		one_philo_handler(philo);
		return (NULL);
	}
	if (philo->data.philo_id % 2 == 0)
		usleep(1000);
	while (!check_if_simulation_should_stop(philo))
	{
		if (check_and_handle_death(philo))
			return (NULL);
		if (try_pick_forks(philo))
		{
			if (check_and_handle_death(philo))
			{
				release_forks(philo);
				return(NULL);
			}
			philosopher_eat(philo);
			release_forks(philo);
			if (philo->data.n_of_times_philos_eat > 0)
			{
				philo->data.n_of_times_philos_eat--;
				if (philo->data.n_of_times_philos_eat == 0)
					return (NULL);
			}
			if (!check_if_simulation_should_stop(philo))
			{
				print_message(&philo->data, "is sleeping");
				long	sleep_start = get_current_time_ms();
				while (get_elapsed_time(sleep_start) < philo->data.time_to_sleep)
				{
					if (check_and_handle_death(philo))
						return (NULL);
					usleep(100);
				}
				if (!check_if_simulation_should_stop(philo))
					print_message(&philo->data, "is thinking");
			}
		}
	}
	return (NULL);
}
