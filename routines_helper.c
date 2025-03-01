/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines_helper.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 22:20:20 by dopereir          #+#    #+#             */
/*   Updated: 2025/03/01 05:17:08 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	one_philo_handler(t_list *philo)
{
	print_message(&philo->data, "has taken a fork");
	usleep(philo->data.time_to_die * 1000);
	print_message(&philo->data, "died");
	stop_simulation(philo);
}

void	stop_simulation(t_list *philo)
{
	if (!philo || !philo->data.simulation_stop || !philo->data.stop_mutex)
		return ;
	pthread_mutex_lock(philo->data.stop_mutex);
	*philo->data.simulation_stop = true;
	pthread_mutex_unlock(philo->data.stop_mutex);
}

bool	helper_pick_forks(t_list *philo)
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
	return (true);
}

bool	helper_pick_forks_rev(t_list *philo)
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
	return (true);
}

int	helper_philo_eat(t_list *philo)
{
	if (check_if_simulation_should_stop(philo))
		return (0);
	philo->data.n_of_times_philos_eat--;
	if (philo->data.n_of_times_philos_eat != 0)
		print_message(&philo->data, "is sleeping");
	if (philo->data.n_of_times_philos_eat == 0)
		return (0);
	return (1);
}
