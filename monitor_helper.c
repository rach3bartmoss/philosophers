/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 21:59:50 by dopereir          #+#    #+#             */
/*   Updated: 2025/03/01 05:39:16 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*monitor_helper_check_death(t_list *current, long current_time)
{
	*(current->data.simulation_stop) = true;
	pthread_mutex_unlock(current->data.stop_mutex);
	pthread_mutex_lock(current->data.print_message);
	printf("%ld %d died\n",
		current_time - current->data.start_time_ms,
		current->data.philo_id);
	pthread_mutex_unlock(current->data.print_message);
	return (NULL);
}

void	*monitor_helper_finish_count(t_list *head)
{
	pthread_mutex_lock(head->data.stop_mutex);
	*(head->data.simulation_stop) = true;
	pthread_mutex_unlock(head->data.stop_mutex);
	return (NULL);
}

int	monitor_iteration_death(t_list *current)
{
	current->data.r_time = get_current_time_ms();
	if ((current->data.r_time - current->data.last_meal_time)
		>= current->data.time_to_die)
	{
		pthread_mutex_lock(current->data.stop_mutex);
		if (!*(current->data.simulation_stop))
		{
			pthread_mutex_unlock(current->data.stop_mutex);
			monitor_helper_check_death(current, current->data.r_time);
			return (1);
		}
		pthread_mutex_unlock(current->data.stop_mutex);
	}
	return (0);
}

t_stat	monitor_helper_process_iteration(t_list *head, int finished_count)
{
	t_list	*current;
	int		i;

	current = head;
	i = 0;
	while (i < head->data.n_philos)
	{
		if (check_if_simulation_should_stop(current))
		{
			current = current->next;
			continue ;
		}
		if (current->data.n_of_times_philos_eat == 0)
			finished_count++;
		else
		{
			if (monitor_iteration_death(current) == 1)
				return (PHILOSOPHER_DIED);
		}
		current = current->next;
		i++;
	}
	if (finished_count == head->data.n_philos)
		return (ALL_FINISHED);
	return (STILL_RUNNING);
}
