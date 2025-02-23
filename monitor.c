/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 23:18:11 by dopereir          #+#    #+#             */
/*   Updated: 2025/02/23 21:56:11 by dopereir         ###   ########.fr       */
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
				ensuting no extra actions from other threads
*/

/*static int	check_philosopher_death(t_list *philo, long current_time)
{
	pthread_mutex_lock(philo->data.stop_mutex);
	if (!*(philo->data.simulation_stop)
		&& (current_time - philo->data.last_meal_time)
		>= philo->data.time_to_die)
	{
		*(philo->data.simulation_stop) = true;
		pthread_mutex_unlock(philo->data.stop_mutex);
		pthread_mutex_lock(philo->data.print_message);
		printf("%ld %d died\n",
			current_time - philo->data.start_time_ms,
			philo->data.philo_id);
		pthread_mutex_unlock(philo->data.print_message);
		return (1);
	}
	pthread_mutex_unlock(philo->data.stop_mutex);
	return (0);
}

static int	process_iteration(t_list *head)
{
	t_list	*current;
	int		i;
	int		finished_count;
	long	current_time;

	i = 0;
	finished_count = 0;
	current = head;
	while (i < head->data.n_philos)
	{
		if (check_if_simulation_should_stop(current))
		{
			current = current->next;
			i++;
			continue ;
		}
		current_time = get_current_time_ms();
		if (check_philosopher_death(current, current_time))
			return (-1);
		if (current->data.n_of_times_philos_eat == 0)
			finished_count++;
		current = current->next;
		i++;
	}
	return (finished_count);
}

void	*monitor_routine(void *arg)
{
	t_list	*head;
	int		finished_count;

	head = (t_list *)arg;
	if (head->data.n_philos == 1)
		return (NULL);
	while (1)
	{
		finished_count = process_iteration(head);
		if (finished_count == -1)
			return (NULL);
		if (finished_count == head->data.n_philos)
		{
			pthread_mutex_lock(head->data.stop_mutex);
			*(head->data.simulation_stop) = true;
			pthread_mutex_unlock(head->data.stop_mutex);
			return (NULL);
		}
		usleep(800);
	}
	return (NULL);
}*/

void	*monitor_routine(void *arg)
{
	t_list	*head;
	t_list	*current;
	long	current_time;
	int		i;
	int		finished_count;

	head = (t_list *)arg;
	if (head->data.n_philos == 1)
		return (NULL);
	while (1)
	{
		current = head;
		i = 0;
		finished_count = 0;
		while (i < head->data.n_philos)
		{
			if (check_if_simulation_should_stop(current))
			{
				current = current->next;
				continue ;
			}
			current_time = get_current_time_ms();
			if ((current_time - current->data.last_meal_time)
				>= current->data.time_to_die)
			{
				pthread_mutex_lock(current->data.stop_mutex);
				if (!*(current->data.simulation_stop))
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
				pthread_mutex_unlock(current->data.stop_mutex);
			}
			if (current->data.n_of_times_philos_eat == 0)
				finished_count++; //added
			current = current->next;
			i++;
		}
		if (finished_count == head->data.n_philos)
		{
			pthread_mutex_lock(head->data.stop_mutex);
			*(head->data.simulation_stop) = true;
			pthread_mutex_unlock(head->data.stop_mutex);
			return (NULL);
		}
		usleep(800);
	}
	return (NULL);
}
