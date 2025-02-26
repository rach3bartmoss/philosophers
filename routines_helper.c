/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines_helper.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 22:20:20 by dopereir          #+#    #+#             */
/*   Updated: 2025/02/26 23:40:44 by dopereir         ###   ########.fr       */
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

bool	check_if_simulation_should_stop(t_list *philo)
{
	bool	should_stop;

	if (!philo || !philo->data.simulation_stop || !philo->data.stop_mutex)
		return (true);
	pthread_mutex_lock(philo->data.stop_mutex);
	should_stop = *philo->data.simulation_stop;
	pthread_mutex_unlock(philo->data.stop_mutex);
	return (should_stop);
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
