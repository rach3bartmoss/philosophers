/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ticket_master.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 11:41:02 by dopereir          #+#    #+#             */
/*   Updated: 2025/03/02 13:53:18 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

unsigned long	get_ticket(t_ticket *ticket)
{
	unsigned long	my_ticket;

	pthread_mutex_lock(&ticket->ticket_mutex);
	my_ticket = ticket->next_ticket;
	ticket->next_ticket++;
	pthread_mutex_unlock(&ticket->ticket_mutex);
	return (my_ticket);
}

void	wait_for_turn(t_ticket *ticket, unsigned long my_ticket)
{
	while (1)
	{
		pthread_mutex_lock(&ticket->ticket_mutex);
		if (ticket->current_ticket == my_ticket)
		{
			pthread_mutex_unlock(&ticket->ticket_mutex);
			break ;
		}
		pthread_mutex_unlock(&ticket->ticket_mutex);
		usleep(50);
	}
}

void	finish_eating(t_ticket *ticket)
{
	pthread_mutex_lock(&ticket->ticket_mutex);
	ticket->current_ticket++;
	pthread_mutex_unlock(&ticket->ticket_mutex);
}
