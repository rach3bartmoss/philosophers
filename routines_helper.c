/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines_helper.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 22:20:20 by dopereir          #+#    #+#             */
/*   Updated: 2025/03/16 02:35:47 by dopereir         ###   ########.fr       */
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

void	stop_simulation(t_list *philo)
{
	if (!philo || !philo->data.simulation_stop || !philo->data.stop_mutex)
		return ;
	pthread_mutex_lock(philo->data.stop_mutex);
	*philo->data.simulation_stop = true;
	pthread_mutex_unlock(philo->data.stop_mutex);
}

bool	helper_pick_forks(t_list *philo, t_list *f_fork, t_list *s_fork)
{
	pthread_mutex_lock(&f_fork->fork);
	if (check_if_simulation_should_stop(philo))
	{
		pthread_mutex_unlock(&f_fork->fork);
		return (false);
	}
	print_message(&philo->data, "has taken a fork");
	pthread_mutex_lock(&s_fork->fork);
	if (check_if_simulation_should_stop(philo))
	{
		pthread_mutex_unlock(&f_fork->fork);
		pthread_mutex_unlock(&s_fork->fork);
		return (false);
	}
	print_message(&philo->data, "has taken a fork");
	return (true);
}

int	helper_philo_eat(t_list *philo)
{
	int	curr_eat_count;

	if (check_if_simulation_should_stop(philo))
		return (0);
	pthread_mutex_lock(philo->data.eat_count_mutex);//
	philo->data.n_of_times_philos_eat--;//here
	curr_eat_count = philo->data.n_of_times_philos_eat;
	pthread_mutex_unlock(philo->data.eat_count_mutex);
	/*if (philo->data.n_of_times_philos_eat != 0)
		print_message(&philo->data, "is sleeping");
	if (philo->data.n_of_times_philos_eat == 0)
		return (0);*/
	if (curr_eat_count != 0)
		print_message(&philo->data, "is sleeping");
	if (curr_eat_count == 0)
		return (0);
	return (1);
}

void	helper_routine_sync(t_list *philo)
{
	long	time_since_last_meal;
	long	time_until_death;
	long	thinking_time;

	print_message(&philo->data, "is thinking");
	time_since_last_meal = get_current_time_ms() - philo->data.last_meal_time;
	time_until_death = philo->data.time_to_die - time_since_last_meal;
	if (time_until_death > philo->data.time_to_die / 2)
	{
		thinking_time = (philo->data.time_to_eat * 100);
		usleep(thinking_time);
	}
	else if (time_until_death > philo->data.time_to_die / 4)
		usleep(500);
}
