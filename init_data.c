/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 21:11:25 by dopereir          #+#    #+#             */
/*   Updated: 2025/02/22 15:49:52 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_main_data(t_data *data, int ac, char **av)
{
	data->n_philos = ft_atoi(av[1]);
	data->time_to_die = ft_atoi(av[2]);
	data->time_to_eat = ft_atoi(av[3]);
	data->time_to_sleep = ft_atoi(av[4]);
	data->philo_id = 0;
	data->start_time_ms = 0;
	if (ac == 6)
		data->n_of_times_philos_eat = ft_atoi(av[5]);
	else
		data->n_of_times_philos_eat = -1;
}

t_data	copy_data(t_data *data)
{
	t_data	new_data;

	memset(&new_data, 0, sizeof(t_data));
	new_data.n_philos = data->n_philos;
	new_data.time_to_die = data->time_to_die;
	new_data.time_to_eat = data->time_to_eat;
	new_data.time_to_sleep = data->time_to_sleep;
	new_data.n_of_times_philos_eat = data->n_of_times_philos_eat;
	new_data.philo_id = (data->philo_id) + 1;
	new_data.print_message = data->print_message;
	new_data.simulation_stop = data->simulation_stop;
	new_data.stop_mutex = data->stop_mutex;
	return (new_data);
}

t_list	*create_node(t_data *data)
{
	t_list	*new_node;

	new_node = (t_list *)malloc(sizeof(t_list));
	if (!new_node)
		return (NULL);
	new_node->data = copy_data(data);
	new_node->data.print_message = data->print_message;
	if (pthread_mutex_init(&(new_node->fork), NULL) != 0)
	{
		free (new_node);
		return (NULL);
	}
	new_node->next = new_node;
	new_node->prev = new_node;
	return (new_node);
}

void	create_circularll_philos(t_list *head, t_data *data, int n)
{
	t_list	*new_node;
	t_list	*current;
	int		i;

	if (!head || !data)
		return ;
	current = head;
	i = 2;
	while (i <= n)
	{
		new_node = create_node(data);
		if (!new_node)
		{
			cleanup_circular_list(head);
			return ;
		}
		new_node->prev = current;
		new_node->next = head;
		current->next = new_node;
		head->prev = new_node;
		current = new_node;
		i++;
	}
}

bool	init_philos_threads(t_list *head, int n_philos)
{
	t_list	*current;
	long	start_time;
	int		philo_num;

	if (!head)
		return (false);
	start_time = get_current_time_ms();
	current = head;
	philo_num = 1;
	while (philo_num <= n_philos)
	{
		current->data.start_time_ms = start_time;
		current->data.last_meal_time = start_time;
		current->data.philo_id = philo_num;
		if (pthread_create(&(current->data.thread_id), NULL,
				philosopher_routine, (void *)current) != 0)
		{
			return (false);
		}
		current = current->next;
		philo_num++;
	}
	return (true);
}
