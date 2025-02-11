/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 21:11:25 by dopereir          #+#    #+#             */
/*   Updated: 2025/02/11 21:14:23 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_data(t_data *data, int ac, char **av)
{
	data->n_philos = ft_atoi(av[1]);
	data->time_to_die = ft_atoi(av[2]);
	data->time_to_eat = ft_atoi(av[3]);
	data->time_to_sleep = ft_atoi(av[4]);
	if (ac == 6)
		data->n_of_times_philos_eat = ft_atoi(av[5]);
}

t_list	*init_philosophers(t_data *data)
{
	t_list	*head;
	t_list	*prev;
	t_list	*node;
	int	i;

	head = NULL;
	prev = NULL;
	i = 0;
	while (i < data->n_philos)
	{
		node = malloc(sizeof(t_list));
		if (!node)
			return (NULL);
		node->data.n_philos = data->n_philos;
		node->data.time_to_die = data->time_to_die;
		node->data.time_to_eat = data->time_to_eat;
		node->data.time_to_sleep = data->time_to_sleep;
		node->data.n_of_times_philos_eat = data->n_of_times_philos_eat;
		node->data.status = THINKING; //default
		if (phthread_mutex_init(&node->fork, NULL) != 0)
		{
			free (node);
			return (NULL);
		}
		node->prev = prev;
		node->next = NULL;
		if (prev)
			prev->next = node;
		else
			head = node;
		prev = node;
		i++;
	}
	head->prev = node;
	node->next = head;
	return (head);
}
