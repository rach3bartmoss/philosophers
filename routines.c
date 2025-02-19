/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 19:45:37 by dopereir          #+#    #+#             */
/*   Updated: 2025/02/19 23:08:40 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	try_pick_forks(t_list *philo)
{
	if (check_if_simulation_should_stop(philo))
		return (false);
	if (philo->data.philo_id % 2 != 0)
	{
		usleep(100);
	}
	if (philo->data.philo_id % 2 == 0)
	{
		pthread_mutex_lock(&philo->fork);
		if (check_if_simulation_should_stop(philo))
		{
			pthread_mutex_unlock(&philo->fork);
			return (false);
		}
		print_message(&philo->data, "has taken a fork");
		pthread_mutex_lock(&philo->prev->fork);
		if (check_if_simulation_should_stop(philo))
		{
			pthread_mutex_unlock(&philo->fork);
			pthread_mutex_unlock(&philo->prev->fork);
			return (false);
		}
		print_message(&philo->data, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(&philo->prev->fork);
		if (check_if_simulation_should_stop(philo))
		{
			pthread_mutex_unlock(&philo->prev->fork);
			return (false);
		}
		print_message(&philo->data, "has taken a fork");
		pthread_mutex_lock(&philo->fork);
		if (check_if_simulation_should_stop(philo))
		{
			pthread_mutex_unlock(&philo->prev->fork);
			pthread_mutex_unlock(&philo->fork);
			return (false);
		}
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
	long	eat_start = get_current_time_ms();
	
	while (get_elapsed_time(eat_start) < philo->data.time_to_eat)
	{
		if (check_if_simulation_should_stop(philo))
			return ;
		long	remain = philo->data.time_to_eat - get_elapsed_time(eat_start);
		if (remain > 1)
			usleep(900);
	}
}

void	philosopher_sleep(t_list *philo)
{
	print_message(&philo->data, "is sleeping");
	long	sleep_start = get_current_time_ms();

	while (get_elapsed_time(sleep_start) < philo->data.time_to_sleep)
	{
		if (check_if_simulation_should_stop(philo))
			return ;
		long	remain = philo->data.time_to_sleep - get_elapsed_time(sleep_start);
		if (remain > 1)
			usleep(900);
	}
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
	while (!check_if_simulation_should_stop(philo))
	{
		if (try_pick_forks(philo))
		{
			if (check_if_simulation_should_stop(philo)) //---------
			{
				release_forks(philo);
				return (NULL);
			}//-------------
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
				philosopher_sleep(philo);
				if (!check_if_simulation_should_stop(philo))
					print_message(&philo->data, "is thinking");
			}
		}
	}
	return (NULL);
}
