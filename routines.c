/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 19:45:37 by dopereir          #+#    #+#             */
/*   Updated: 2025/02/12 22:17:27 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//here we must have all the behaviour of the philosophers
//1)we must init the threads for each philo
//	create forks, if a philo is has the two forks at disposal it eats
//	how to create a data structutes to acomodate the philos at it forks
// DEADLOCK CAUTION by make one philosopher lock 
//the fork in reverse order, or create  ordering rule.

bool	try_pick_forks(t_list *philo)
{
	if (philo->data.philo_id % 2 == 0)
	{
		pthread_mutex_lock(&philo->fork);
		print_message(philo, "has taken a fork");
		pthread_mutex_lock(&philo->prev->fork);
		print_message(philo, "has taken a fork");
	}
	else
	{
		usleep(100);
		pthread_mutex_lock(&philo->prev->fork);
		print_message(philo, "has taken a fork");
		pthread_mutex_lock(&philo->fork);
		print_message(philo, "has taken a fork");
	}
	return (true);
}

void	release_forks(t_list *philo)
{
	pthread_mutex_unlock(&philo->fork);
	pthread_mutex_unlock(&philo->prev->fork);
}

void	philosopher_eat(t_list *philo)
{
	print_action(philo, "is eating");
	philo->data.last_meal_time = get_current_time_ms();
	usleep(philo->data.time_to_eat * 1000);
}

void	*philosopher_routine(void *arg)
{
	t_list	*philo;

	philo = (t_list *)arg;
	philo->data.last_meal_time = get_current_time_ms();
	if (philo->data.philo_id % 2 == 0)
		usleep(1000);
	while (1)
	{
		if (get_elapsed_time(philo->data.last_meal_time)
			> philo->data.time_to_die)
		{
			print_message(philo, "died");
			return (NULL);
		}
		print_message(philo, "is thinking");
		if (try_pick_forks(philo))
		{
			philosopher_eat(philo);
			release_forks(philo);
			if (philo->data.n_of_times_philos_eat > 0)
			{
				philo->data.n_of_times_philos_eat--;
				if (philo->data.n_of_times_philos_eat == 0)
					return (NULL);
			}
			print_message(philo, "is sleeping");
			usleep(philo->data.time_to_sleep * 1000);
		}
	}
	return (NULL);
}
