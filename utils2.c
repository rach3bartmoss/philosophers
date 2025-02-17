/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 22:02:40 by dopereir          #+#    #+#             */
/*   Updated: 2025/02/17 00:49:09 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

void	stop_simulation(t_list *philo)
{
	if (!philo || !philo->data.simulation_stop || !philo->data.stop_mutex)
		return ;
	pthread_mutex_lock(philo->data.stop_mutex);
	*philo->data.simulation_stop = true;
	pthread_mutex_unlock(philo->data.stop_mutex);
}

bool	check_and_handle_death(t_list *philo)
{
	if (get_elapsed_time(philo->data.last_meal_time) > philo->data.time_to_die)
	{
		pthread_mutex_lock(philo->data.stop_mutex);
		if (!*philo->data.simulation_stop)
		{
			*philo->data.simulation_stop = true;
			pthread_mutex_unlock(philo->data.stop_mutex);
			print_message(&philo->data, "died");
		}
		else
		{
			pthread_mutex_unlock(philo->data.stop_mutex);
		}
		return (true);
	}
	return (false);
}
