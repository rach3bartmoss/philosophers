/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines_helper.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 22:20:20 by dopereir          #+#    #+#             */
/*   Updated: 2025/02/19 21:16:31 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	one_philo_handler(t_list *philo)
{
	print_message(&philo->data, "has taken a fork");
	usleep(philo->data.time_to_die * 1000);
	print_message(&philo->data, "died");
	stop_simulation(philo);
}