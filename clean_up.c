/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_up.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 20:31:52 by dopereir          #+#    #+#             */
/*   Updated: 2025/03/03 18:26:52 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	cleanup_node(t_list *node)
{
	if (!node)
		return ;
	pthread_mutex_destroy(&node->fork);
	free(node);
}

void	cleanup_circular_list(t_list *head)
{
	t_list	*current;
	t_list	*next;

	if (!head)
		return ;
	current = head->next;
	head->next = NULL;
	while (current && current != head)
	{
		next = current->next;
		cleanup_node(current);
		current = next;
	}
	cleanup_node(head);
}

void	cleanup_threads(t_list *head, int n_philos)
{
	t_list	*current;
	int		i;

	if (!head)
		return ;
	current = head;
	i = 0;
	while (i < n_philos)
	{
		pthread_join(current->data.thread_id, NULL);
		current = current->next;
		i++;
	}
}

void	cleanup_all(t_list *head, t_shared_mut *main_mutexes,
	t_data *main_data)
{
	if (head)
		cleanup_threads(head, main_data->n_philos);
	cleanup_circular_list(head);
	if (main_mutexes)
	{
		pthread_mutex_destroy(&main_mutexes->print_mutex);
		pthread_mutex_destroy(&main_mutexes->stop_mutex);
		free(main_mutexes);
	}
	if (main_data)
	{
		if (main_data->simulation_stop)
		{
			free(main_data->simulation_stop);
			main_data->simulation_stop = NULL;
		}
		free(main_data);
		main_data = NULL;
	}
}
