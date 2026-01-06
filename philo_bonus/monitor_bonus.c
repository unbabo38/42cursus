#include "philo_bonus.h"

void	*monitor(void *arg)
{
	t_philo	*philo;
	t_data	*data;
	long long	time;

	philo = (t_philo *)arg;
	data = philo->data;
	while (1)
	{
		sem_wait(data->meal_sem);
		time = get_time() - philo->last_meal_time;
		if (time > data->time_to_die)
		{
			sem_post(data->meal_sem);
			sem_wait(data->print_sem);
			printf("%lld %d died\n", get_time() - data->start_time, philo->id);
			sem_post(data->print_sem);
			sem_post(data->dead_sem);
			exit(1);
		}
		sem_post(data->meal_sem);
		if (data->num_meals != -1 && philo->meals_eaten >= data->num_meals)
			exit(0);
		usleep(1000);
	}
	return (NULL);
}

