/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 18:53:26 by dopereir          #+#    #+#             */
/*   Updated: 2025/03/01 06:09:50 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(const char *str)
{
	int	sign;
	int	result;
	int	i;

	sign = 1;
	result = 0;
	i = 0;
	while (str[i] == ' ' || (str[i] >= '\t' && str[i] <= '\r'))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (sign * result);
}

long long	get_current_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	print_message(t_data *data, const char *action)
{
	pthread_mutex_lock(data->print_message);
	printf("%lld %d %s\n",
		get_current_time_ms() - data->start_time_ms,
		data->philo_id, action);
	pthread_mutex_unlock(data->print_message);
}

long long	get_elapsed_time(long start_time_ms)
{
	return (get_current_time_ms() - start_time_ms);
}

bool	check_if_simulation_should_stop(t_list *philo)
{
	bool	should_stop;

	if (!philo || !philo->data.simulation_stop || !philo->data.stop_mutex)
		return (true);
	pthread_mutex_lock(philo->data.stop_mutex);
	should_stop = *philo->data.simulation_stop;
	pthread_mutex_unlock(philo->data.stop_mutex);
	return (should_stop);
}
