/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_up.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 20:31:52 by dopereir          #+#    #+#             */
/*   Updated: 2025/02/13 22:17:30 by dopereir         ###   ########.fr       */
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

void	cleanup_all(t_list *head, pthread_mutex_t *print_message,
	t_data *main_data)
{
	if (head)
	{
		cleanup_threads(head, main_data->n_philos);
		cleanup_circular_list(head);
	}
	if (print_message)
		pthread_mutex_destroy(print_message);
	if (main_data->stop_mutex)
		pthread_mutex_destroy(main_data->stop_mutex);
	if (main_data)
		free(main_data);
}
