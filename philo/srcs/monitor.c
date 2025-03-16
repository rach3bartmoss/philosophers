/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 23:18:11 by dopereir          #+#    #+#             */
/*   Updated: 2025/03/16 03:27:02 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//high level abstraction for convenient and effective process synchronization
//the monitor construct ensures that only one process at a time can be active
//within the monitor
/*
	1) iterate over the list
	2) compare current philosopher last_meal_time to the current_time
	3) if exceds time_to_die;
	4) immediately set a shared flag / log the death?


	make sure the monitor read the last_meal_time or status
	uses appropriate mutex locks
	this avoids race conditions

	-> 4) -> 5) with a shared flag, the philos routine must always
				check that flag
				if flag is set, the philosopher could gracefully exit
				ensuting no extra actions from other threads*/

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
	pthread_mutex_lock(current->data.stop_mutex);
	if ((current->data.r_time - current->data.last_meal_time)
		>= current->data.time_to_die)
	{
		if (!*(current->data.simulation_stop))
		{
			monitor_helper_check_death(current, current->data.r_time);
			return (1);
		}
	}
	pthread_mutex_unlock(current->data.stop_mutex);
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
		if (monitor_eat_lock_helper(current) == 0)
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

void	*monitor_routine(void *arg)
{
	t_list	*head;
	t_stat	status;
	int		finished_count;

	finished_count = 0;
	head = (t_list *)arg;
	if (head->data.n_philos == 1)
		return (NULL);
	while (1)
	{
		status = monitor_helper_process_iteration(head, finished_count);
		if (status == PHILOSOPHER_DIED)
			return (NULL);
		if (status == ALL_FINISHED)
			return (monitor_helper_finish_count(head));
		usleep(800);
	}
	return (NULL);
}
