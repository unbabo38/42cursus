#include "philo_bonus.h"

void	philo_routine(t_philo *philo)
{
	t_data	*data;
	pthread_t	monitor_thread;
	int		finished;

	data = philo->data;
	if (pthread_create(&monitor_thread, NULL, monitor, philo) != 0)
		exit(1);
	if (philo->id % 2 == 0)
		usleep(1000);
	finished = 0;
	while (1)
	{
		sem_wait(data->meal_sem);
		if (data->num_meals != -1 && philo->meals_eaten >= data->num_meals)
			finished = 1;
		sem_post(data->meal_sem);
		if (finished)
			break ;
		eat(philo);
		sleep_think(philo);
	}
	pthread_join(monitor_thread, NULL);
	exit(0);
}

