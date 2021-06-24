/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jacher <jacher@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/24 15:07:35 by jacher            #+#    #+#             */
/*   Updated: 2021/06/24 19:03:57 by jacher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	ft_free_help(t_philo *philo, pthread_mutex_t *forks, int res)
{
	if (forks)
		free(forks);
	if (philo)
		free(philo);
	return (res);
}

void init_philosophers(t_philo *philo, t_d *d, pthread_mutex_t *forks)
{
	int i;

	i = 0;
	while (i < d->n_philo)
	{
		philo[i].num = i + 1;
		philo[i].status = THINK;
		philo[i].d = d;
		if (i != 0)
			philo[i].num_fork_l = i;
		else
			philo[i].num_fork_l = d->n_philo;
		philo[i].num_fork_r = i + 1;
		philo[i].nb_meals = 0;
		philo[i].time_last_meal = 0;
		philo[i].bol_death = 0;
		philo[i].forks = forks;
		i++;
	}
}

void *routine(void *arg)
{
	t_philo *philo;
	philo = (t_philo *)arg;

	while (philo->status == THINK)
	{
		pthread_mutex_lock(&(philo->forks[philo->num_fork_l - 1]));
		pthread_mutex_lock(&(philo->forks[philo->num_fork_r - 1]));
		philo->status = EAT;
		print_time_stamp();
		printf("Philo %d is eating\n", philo->num);
		usleep(philo->d->time_to_eat);
		pthread_mutex_unlock(&(philo->forks[philo->num_fork_l -1]));
		pthread_mutex_unlock(&(philo->forks[philo->num_fork_r -1]));
		philo->status = SLEEP;
		print_time_stamp();
		printf("Philo %d is sleeping\n", philo->num);
		usleep(philo->d->time_to_sleep);
		philo->status = THINK;
		print_time_stamp();
		printf("Philo %d is thinking\n", philo->num);
	}
	/*printf("*****\nOne philosophe has been created / done routine\n");
	print_philo(*philo);
	if (philo
	printf("************\n");*/
	return (NULL);
}

int launch_philo(t_d *d)
{
	unsigned int	i;
	t_philo			*philo;
	pthread_mutex_t	*forks;

	philo = malloc(sizeof(t_philo) * d->n_philo);
	if (philo == NULL)
		return (-1);
	forks = malloc(sizeof(pthread_mutex_t) * d->n_philo);
	if (forks == NULL)
		return (ft_free_help(philo, NULL, -1));
	init_philosophers(philo, d, forks);
	i = 0;
	while (i < d->n_philo)
	{
		if (pthread_create(&(philo[i].thread), NULL, &routine, (void *)(&(philo[i]))) != 0)
			return (ft_free_help(philo, forks, -1));
		i++;
	}
	i = 0;
	while (i < d->n_philo)
	{
		if (pthread_join(philo[i].thread, NULL) != 0)
			return (ft_free_help(philo, forks, -1));
		i++;
	}
	return (ft_free_help(philo, forks, 0));
}
		

