/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_help.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jacher <jacher@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/24 15:07:35 by jacher            #+#    #+#             */
/*   Updated: 2021/07/04 08:18:35 by jacher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	ft_free_help(t_philo *philo, pthread_mutex_t *forks_mutex, int *forks_table, int res)
{
	if (forks_table)
		free(forks_table);
	if (forks_mutex)
		free(forks_mutex);
	if (philo)
		free(philo);
	return (res);
}

int		ft_destroy(pthread_mutex_t *forks_mutex, t_d *d, unsigned int lim, unsigned int nb)
{
	unsigned int i;

	i = 0;
	while (i < lim)
	{
		pthread_mutex_destroy(&(forks_mutex[i]));
		i++;
	}
	if (nb > 0)
		pthread_mutex_destroy(&(d->print_mutex));
	if (nb > 1)
		pthread_mutex_destroy(&(d->check_mutex));
	if (nb > 2)
		pthread_mutex_destroy(&(d->meal_mutex));
	if (nb > 3)
		pthread_mutex_destroy(&(d->died_mutex));
	return (-1);
}

void init_philosophers(t_philo *philo, t_d *d)
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
		philo[i].num_forks = 0;
		philo[i].bol_thread = 0;
		philo[i].nb_meals = 0;
		i++;
	}
}

int create_threads(t_philo *philo, pthread_mutex_t *forks_mutex, int *forks_table)
{
	int i;

	i = 0;
	while (i < philo->d->n_philo)
	{
		if (pthread_create(&(philo[i].thread), NULL, &routine, (void *)(&(philo[i]))) != 0)
			return (ft_free_help(philo, forks_mutex, forks_table, -1));
		i += 2;
	}
	usleep(5000);
	i = 1;
	while (i < philo->d->n_philo)
	{
		if (pthread_create(&(philo[i].thread), NULL, &routine, (void *)(&(philo[i]))) != 0)
			return (ft_free_help(philo, forks_mutex, forks_table, -1));
		i += 2;
	}
	return (0);
}

int	launch_philo_init(t_d *d, t_philo *philo, pthread_mutex_t *forks_mutex,
						int *forks_table)
{
	unsigned int i;

	i = 0;
	while (i < d->n_philo)
	{
		if (pthread_mutex_init(&(forks_mutex[i]), NULL) != 0)
			return (ft_destroy(forks_mutex, d, i, 0));
		forks_table[i] = 0;
		i++;
	}
	d->forks_mutex = forks_mutex;
	d->forks_table = forks_table;
	if (pthread_mutex_init(&(d->print_mutex), NULL) != 0)
		return (ft_destroy(forks_mutex, d, i, 1));
	if(pthread_mutex_init(&(d->check_mutex), NULL) != 0)
		return (ft_destroy(forks_mutex, d, i, 2));
	if(pthread_mutex_init(&(d->meal_mutex), NULL) != 0)
		return (ft_destroy(forks_mutex, d, i, 3));
	if(pthread_mutex_init(&(d->died_mutex), NULL) != 0)
		return(ft_destroy(forks_mutex, d, i, 4));
	init_philosophers(philo, d);
	gettimeofday(&(d->begin), NULL);
	return (0);
}
