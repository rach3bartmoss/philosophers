/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 22:02:40 by dopereir          #+#    #+#             */
/*   Updated: 2025/03/03 18:46:38 by dopereir         ###   ########.fr       */
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
