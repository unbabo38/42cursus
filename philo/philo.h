/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmura <tmura@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 13:30:33 by tmura             #+#    #+#             */
/*   Updated: 2026/01/07 13:30:33 by tmura            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <stdbool.h>

# define MAX_PHILOS 200

typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	long long		last_meal_time;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	struct s_data	*data;
}	t_philo;

typedef struct s_data
{
	int				num_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				num_meals;
	bool			dead_flag;
	bool			all_ate;
	long long		start_time;
	pthread_mutex_t	forks[MAX_PHILOS];
	pthread_mutex_t	meal_check;
	pthread_mutex_t	print_mutex;
	t_philo			philos[MAX_PHILOS];
}	t_data;

// utils.c
long long	get_time(void);
void		print_action(t_data *data, int id, char *str);
int			ft_atoi(const char *str);
int			error_msg(char *str);

// init.c
int			init_data(t_data *data, int argc, char **argv);
int			init_philos(t_data *data);
int			init_mutexes(t_data *data);

// actions.c
void		eat(t_philo *philo);
void		sleep_think(t_philo *philo);

// monitor.c
void		*monitor(void *arg);
int			check_death(t_data *data);
int			check_all_ate(t_data *data);

// routine.c
void		*philo_routine(void *arg);

#endif
