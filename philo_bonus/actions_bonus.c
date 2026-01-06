#include "philo_bonus.h"

void	eat(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	sem_wait(data->forks);
	print_action(data, philo->id, "has taken a fork");
	if (data->num_philos == 1)
	{
		usleep(data->time_to_die * 1000);
		sem_post(data->forks);
		return ;
	}
	sem_wait(data->forks);
	print_action(data, philo->id, "has taken a fork");
	print_action(data, philo->id, "is eating");
	sem_wait(data->meal_sem);
	philo->last_meal_time = get_time();
	philo->meals_eaten++;
	sem_post(data->meal_sem);
	usleep(data->time_to_eat * 1000);
	sem_post(data->forks);
	sem_post(data->forks);
}

void	sleep_think(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	print_action(data, philo->id, "is sleeping");
	usleep(data->time_to_sleep * 1000);
	print_action(data, philo->id, "is thinking");
}

