/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 21:02:27 by dopereir          #+#    #+#             */
/*   Updated: 2025/03/02 15:50:54 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_isnum(char c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

static int	check_arg(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (ft_isnum(str[i]) == 0)
			return (1);
		i++;
	}
	return (0);
}

static bool	validate_input(int ac, char **av)
{
	int	i;
	int	value;

	if (ac != 5 && ac != 6)
		return (printf("Error\nInvalid number of arguments\n"), false);
	i = 1;
	while (i < ac)
	{
		if (check_arg(av[i]) == 1)
			return (printf("Error\nInput contains non-number character\n"),
				false);
		value = ft_atoi(av[i]);
		if (value <= 0)
			return (printf("Error\nAll arguments must be positive numbers\n"),
				false);
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
	t_shared_mut	*main_mutexes;
	pthread_t		monitor_thread;


	if (!validate_input(ac, av))
		return (1);
	main_data = malloc(sizeof(t_data));
	if (!main_data)
	{
		printf("Error\nMain data allocation failed\n");
		return (1);
	}
	main_mutexes = main_mutexes_init(main_data);
	if (!main_mutexes)
	{
		free(main_data);
		return (1);
	}
	init_main_data(main_data, ac, av);
	head = create_node(main_data);
	if (!head)
	{
		cleanup_all(NULL, main_mutexes, main_data);
		return (1);
	}
	create_circularll_philos(head, main_data, main_data->n_philos);
	if (!init_philos_threads(head, main_data->n_philos))
	{
		cleanup_all(head, main_mutexes, main_data);
		return (1);
	}
	pthread_create(&monitor_thread, NULL, monitor_routine, head);
	pthread_join(monitor_thread, NULL);
	cleanup_all(head, main_mutexes, main_data);
	return (0);
}
