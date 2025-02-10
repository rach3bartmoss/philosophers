/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 21:02:27 by dopereir          #+#    #+#             */
/*   Updated: 2025/02/09 22:16:48 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int ac, char **av)
{
	if (ac < 4)
	{
		printf("Error\nInsuficient Arguments\n");
		return (0);
	}

	t_data	data = {0};

	int	i = 1;
	while (i < ac)
	{
		printf("%d\n", atoi(av[i]));
		i++;
	}
	init_data(&data, ac, av);

	printf("%d\n", data.n_philos);
	printf("%d\n", data.time_to_die);
	printf("%d\n", data.time_to_eat);
	printf("%d\n", data.time_to_sleep);
	printf("%d\n", data.n_of_times_philos_eat);
}
