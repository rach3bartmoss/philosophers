/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 12:00:25 by dopereir          #+#    #+#             */
/*   Updated: 2025/03/02 17:10:29 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_shared_mut	*main_mutexes_init(t_data *main_data)
{
	t_ticket		*ticket;
	t_shared_mut	*main_muts;

	ticket = malloc(sizeof(t_ticket));
	main_muts = malloc(sizeof(t_shared_mut));
	if (!ticket)
		return (printf("Error\nTicket allocation failed\n"), NULL);
	if (!main_muts)
	{
		free(ticket);
		printf("Error\nMain mutexes allocation failed\n");
		return (NULL);
	}
	if (pthread_mutex_init(&ticket->ticket_mutex, NULL) != 0)
	{
		free(main_muts);
		free(ticket);
		return (NULL);
	}
	if (pthread_mutex_init(&main_muts->print_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&ticket->ticket_mutex);
		free(main_muts);
		free(ticket);
		printf("Error\nFailed to initialize print mutex\n");
		return (NULL);
	}
	if (pthread_mutex_init(&main_muts->stop_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&ticket->ticket_mutex);
		pthread_mutex_destroy(&main_muts->print_mutex);
		free(main_muts);
		free(ticket);
		printf("Error\nFailed to initialize stop_mutex\n");
		return (NULL);
	}
	ticket->next_ticket = 0;
	ticket->current_ticket = 0;
	main_data->ticket_master = ticket;
	main_data->simulation_stop = malloc(sizeof(bool));
	*(main_data->simulation_stop) = false;
	main_data->stop_mutex = &main_muts->stop_mutex;
	main_data->print_message = &main_muts->print_mutex;

	return (main_muts);
}