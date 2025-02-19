#include "philo.h"

//high level abstraction for convenient and effective process synchronization
//the monitor construct ensures that only one process at a time can be active
//within the monitor
	/*
	1) iterate over the list
	2) compare current philosopher last_meal_time to the current_time
	3) if exceds time_to_die;
	4) immediately set a shared flag / log the death?


	make sure the monitor read the last_meal_time or status
	uses appropriate mutex locks
	this avoids race conditions

	-> 4) -> 5) with a shared flag, the philos routine must always check that flag
	if flag is set, the philosopher could gracefully exit ensuting no extra actions
	from other threads
	*/

void	*monitor_routine(void *arg)
{
	t_list	*head;
	t_list	*current;
	long	current_time;

	head = (t_list *)arg;
	while (1)
	{
		current = head;
		for (int i = 0; i < head->data.n_philos; i++)
		{
			if (check_if_simulation_should_stop(current))
			{
				current = current->next;
				continue ;
			}
			current_time = get_current_time_ms();
			if ((current_time - current->data.last_meal_time) >= current->data.time_to_die)
			{
				pthread_mutex_lock(current->data.stop_mutex);
				if (!*(current->data.simulation_stop))
				{
					*(current->data.simulation_stop) = true;
					pthread_mutex_unlock(current->data.stop_mutex);
					pthread_mutex_lock(current->data.print_message);
					printf("%ld %d died\n",
						current_time - current->data.start_time_ms,
						current->data.philo_id);
					pthread_mutex_unlock(current->data.print_message);
					return (NULL);
				}
				pthread_mutex_unlock(current->data.stop_mutex);
			}
			current = current->next;
		}
		usleep(800);
	}
	return (NULL);
}
