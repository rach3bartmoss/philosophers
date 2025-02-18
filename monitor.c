#include "philo.h"

//high level abstraction for convenient and effective process synchronization
//the monitor construct ensures that only one process at a time can be active
//within the monitor
void	monitor(t_list *philo, t_monitor monitor)
{
	/*
	philosopher[i] can only set its state to eating when the previous
	fork is and its own fork are available
	(philo->data.status != eating && philo->prev->data.status != eating)

	*/
}
