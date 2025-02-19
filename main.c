/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 21:02:27 by dopereir          #+#    #+#             */
/*   Updated: 2025/02/19 23:02:16 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	validate_input(int ac, char **av)
{
	int	i;
	int	value;

	if (ac != 5 && ac != 6)
	{
		printf("Error\nInvalid number of arguments\n");
		return (false);
	}
	i = 1;
	while (i < ac)
	{
		value = ft_atoi(av[i]);
		if (value <= 0)
		{
			printf("Error\nAll arguments must be positive numbers\n");
			return (false);
		}
		i++;
	}
	if (ft_atoi(av[1]) > 200)
	{
		printf("Error\nMaximum number of philosophers is 200\n");
		return (false);
	}
	return (true);
}

int	main(int ac, char **av)
{
	t_data			*main_data;
	t_list			*head;
	bool			simulation_stop;
	pthread_mutex_t	stop_mutex;
	pthread_mutex_t	print_mutex;
	pthread_t		monitor_thread;

	if (!validate_input(ac, av))
		return (1);
	if (pthread_mutex_init(&print_mutex, NULL) != 0)
	{
		printf("Error\nFailed to initialize print mutex\n");
		return (1);
	}
	if (pthread_mutex_init(&stop_mutex, NULL) != 0)
	{
		printf("Error\nFailed to initialize stop_mutex\n");
		return (1);
	}
	simulation_stop = false;
	main_data = malloc(sizeof(t_data));
	if (!main_data)
	{
		pthread_mutex_destroy(&print_mutex);
		pthread_mutex_destroy(&stop_mutex);
		return (1);
	}
	main_data->simulation_stop = &simulation_stop;
	main_data->stop_mutex = &stop_mutex;
	init_main_data(main_data, ac, av);
	main_data->print_message = &print_mutex;
	head = create_node(main_data);
	if (!head)
	{
		cleanup_all(NULL, &print_mutex, main_data);
		return (1);
	}
	create_circularll_philos(head, main_data, main_data->n_philos);
	if (!init_philos_threads(head, main_data->n_philos))
	{
		cleanup_all(head, &print_mutex, main_data);
		return (1);
	}
	pthread_create(&monitor_thread, NULL, monitor_routine, head);
	pthread_join(monitor_thread, NULL);
	//cleanup_threads(head, main_data->n_philos);
	cleanup_all(head, &print_mutex, main_data);
	return (0);
}
