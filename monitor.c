/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 23:18:11 by dopereir          #+#    #+#             */
/*   Updated: 2025/02/25 00:00:29 by dopereir         ###   ########.fr       */
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

void	*monitor_routine(void *arg)
{
	t_list	*head;
	t_list	*current;
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
		if (monitor_helper_process_iteration(current, head, i, finished_count))
			return (NULL);
		if (finished_count == head->data.n_philos)
			return (monitor_helper_finish_count(head));
		usleep(800);
	}
	return (NULL);
}
