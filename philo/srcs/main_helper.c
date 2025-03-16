/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 12:00:25 by dopereir          #+#    #+#             */
/*   Updated: 2025/03/16 03:03:13 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	helper_main_mutexes_init(t_shared_mut *main_muts)
{
	if (pthread_mutex_init(&main_muts->eat_count_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&main_muts->print_mutex);
		pthread_mutex_destroy(&main_muts->stop_mutex);
		free(main_muts);
		printf("Error\nFailed to initialize eat_count_mutex\n");
		return (1);
	}
	return (0);
}

t_shared_mut	*main_mutexes_init(t_data *main_data)
{
	t_shared_mut	*main_muts;

	main_muts = malloc(sizeof(t_shared_mut));
	if (!main_muts)
		return (printf("Error\nMain mutexes allocation failed\n"), NULL);
	if (pthread_mutex_init(&main_muts->print_mutex, NULL) != 0)
	{
		free(main_muts);
		return (printf("Error\nPrint mutex init failed\n"), NULL);
	}
	if (pthread_mutex_init(&main_muts->stop_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&main_muts->print_mutex);
		free(main_muts);
		printf("Error\nFailed to initialize stop_mutex\n");
		return (NULL);
	}
	if (helper_main_mutexes_init(main_muts) == 1)
		return (NULL);
	main_data->simulation_stop = malloc(sizeof(bool));
	*(main_data->simulation_stop) = false;
	main_data->stop_mutex = &main_muts->stop_mutex;
	main_data->print_message = &main_muts->print_mutex;
	main_data->eat_count_mutex = &main_muts->eat_count_mutex;
	return (main_muts);
}

t_shared_mut	*main_checks(t_data *main_data, t_shared_mut *main_mutexes)
{
	if (!main_data)
	{
		printf("Error\nMain data allocation failed\n");
		return (NULL);
	}
	main_mutexes = main_mutexes_init(main_data);
	if (!main_mutexes)
	{
		free(main_data);
		return (NULL);
	}
	return (main_mutexes);
}

bool	main_trigged(t_list *head, t_data *main_data,
						t_shared_mut *main_mutexes)
{
	if (!head)
	{
		cleanup_all(NULL, main_mutexes, main_data);
		return (false);
	}
	create_circularll_philos(head, main_data, main_data->n_philos);
	if (!init_philos_threads(head, main_data->n_philos))
	{
		cleanup_all(head, main_mutexes, main_data);
		return (false);
	}
	return (true);
}
