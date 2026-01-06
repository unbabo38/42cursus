#include "philo.h"

void	*philo_routine(void *arg)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)arg;
	data = philo->data;
	if (philo->id % 2 == 0)
		usleep(1000);
	while (1)
	{
		pthread_mutex_lock(&data->meal_check);
		if (data->dead_flag || data->all_ate)
		{
			pthread_mutex_unlock(&data->meal_check);
			break ;
		}
		if (data->num_meals != -1 && philo->meals_eaten >= data->num_meals)
		{
			pthread_mutex_unlock(&data->meal_check);
			break ;
		}
		pthread_mutex_unlock(&data->meal_check);
		eat(philo);
		pthread_mutex_lock(&data->meal_check);
		if (data->dead_flag || data->all_ate)
		{
			pthread_mutex_unlock(&data->meal_check);
			break ;
		}
		pthread_mutex_unlock(&data->meal_check);
		sleep_think(philo);
		pthread_mutex_lock(&data->meal_check);
		if (data->dead_flag || data->all_ate)
		{
			pthread_mutex_unlock(&data->meal_check);
			break ;
		}
		pthread_mutex_unlock(&data->meal_check);
	}
	return (NULL);
}

