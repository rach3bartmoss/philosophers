/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 22:02:40 by dopereir          #+#    #+#             */
/*   Updated: 2025/03/16 03:18:46 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	helper_routine_main_iteration(t_list *philo)
{
	if (check_if_simulation_should_stop(philo))
	{
		release_forks(philo);
		return (false);
	}
	philosopher_eat(philo);
	release_forks(philo);
	if (philo->data.n_of_times_philos_eat == 0)
		return (false);
	return (true);
}

void	routine_meal_lock_helper(t_list *philo)
{
	pthread_mutex_lock(philo->data.eat_count_mutex);
	philo->data.last_meal_time = get_current_time_ms();
	pthread_mutex_unlock(philo->data.eat_count_mutex);
}

void	routine_stop_lock_helper(t_list *philo)
{
	pthread_mutex_lock(philo->data.stop_mutex);
	philo->data.last_meal_time = get_current_time_ms();
	pthread_mutex_unlock(philo->data.stop_mutex);
}

long	monitor_eat_lock_helper(t_list *current)
{
	long	times;

	times = 0;
	pthread_mutex_lock(current->data.eat_count_mutex);
	times = current->data.n_of_times_philos_eat;
	pthread_mutex_unlock(current->data.eat_count_mutex);
	return (times);
}
