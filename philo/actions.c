#include "philo.h"

void	eat(t_philo *philo)
{
	t_data	*data;
	long long	start_time;
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;

	data = philo->data;
	pthread_mutex_lock(&data->meal_check);
	if (data->dead_flag || data->all_ate)
	{
		pthread_mutex_unlock(&data->meal_check);
		return ;
	}
	pthread_mutex_unlock(&data->meal_check);
	if (philo->id % 2 == 0)
	{
		first_fork = philo->left_fork;
		second_fork = philo->right_fork;
	}
	else
	{
		first_fork = philo->right_fork;
		second_fork = philo->left_fork;
	}
	pthread_mutex_lock(first_fork);
	print_action(data, philo->id, "has taken a fork");
	if (data->num_philos == 1)
	{
		usleep(data->time_to_die * 1000);
		pthread_mutex_unlock(first_fork);
		return ;
	}
	pthread_mutex_lock(&data->meal_check);
	if (data->dead_flag || data->all_ate)
	{
		pthread_mutex_unlock(&data->meal_check);
		pthread_mutex_unlock(first_fork);
		return ;
	}
	pthread_mutex_unlock(&data->meal_check);
	pthread_mutex_lock(second_fork);
	print_action(data, philo->id, "has taken a fork");
	pthread_mutex_lock(&data->meal_check);
	if (data->dead_flag || data->all_ate)
	{
		pthread_mutex_unlock(&data->meal_check);
		pthread_mutex_unlock(first_fork);
		pthread_mutex_unlock(second_fork);
		return ;
	}
	philo->last_meal_time = get_time();
	philo->meals_eaten++;
	pthread_mutex_unlock(&data->meal_check);
	print_action(data, philo->id, "is eating");
	start_time = get_time();
	while (get_time() - start_time < data->time_to_eat)
	{
		pthread_mutex_lock(&data->meal_check);
		if (data->dead_flag || data->all_ate)
		{
			pthread_mutex_unlock(&data->meal_check);
			pthread_mutex_unlock(first_fork);
			pthread_mutex_unlock(second_fork);
			return ;
		}
		pthread_mutex_unlock(&data->meal_check);
		usleep(100);
	}
	pthread_mutex_unlock(first_fork);
	pthread_mutex_unlock(second_fork);
}

void	sleep_think(t_philo *philo)
{
	t_data	*data;
	long long	start_time;

	data = philo->data;
	print_action(data, philo->id, "is sleeping");
	start_time = get_time();
	while (get_time() - start_time < data->time_to_sleep)
	{
		pthread_mutex_lock(&data->meal_check);
		if (data->dead_flag || data->all_ate)
		{
			pthread_mutex_unlock(&data->meal_check);
			return ;
		}
		pthread_mutex_unlock(&data->meal_check);
		usleep(100);
	}
	print_action(data, philo->id, "is thinking");
}

