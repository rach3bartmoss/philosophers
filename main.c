/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 21:02:27 by dopereir          #+#    #+#             */
/*   Updated: 2025/02/10 22:03:01 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int ac, char **av)
{
	t_data		data = {0};

	init_data(&data, ac, av);

	int			i;
	int			thread_args[data.n_philos];
	int			result_code;
	pthread_t	threads[data.n_philos];

	if (ac < 4)
	{
		printf("Error\nInsuficient Arguments\n");
		return (0);
	}

	i = 0;
	while (i < data.n_philos)
	{
		thread_args[i] = i;
		result_code = pthread_create(&threads[i], NULL,
			thread_function, (void *)&thread_args[i]);
		if (result_code != 0)
		{
			fprintf(stderr, "Error creating thread %d: %d\n", i, result_code);
			exit(EXIT_FAILURE);
		}
		i++;
	}
	i = 0;
	while (i < data.n_philos)
	{
		result_code = pthread_join(threads[i], NULL);
		if (result_code != 0)
		{
			fprintf(stderr, "Error joining thread %d: %d\n", i, result_code);
			exit(EXIT_FAILURE);
		}
		i++;
	}
	printf("All threads have completed execution.\n");
}
