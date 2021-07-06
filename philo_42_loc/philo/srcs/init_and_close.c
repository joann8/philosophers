/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_and_close.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jacher <jacher@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/24 15:07:35 by jacher            #+#    #+#             */
/*   Updated: 2021/07/05 13:10:46 by jacher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	ft_free_help(t_philo *philo, pthread_mutex_t *forks_mutex, int res)
{
	if (forks_mutex)
		free(forks_mutex);
	if (philo)
		free(philo);
	return (res);
}

int	ft_destroy(pthread_mutex_t *forks_mutex, t_d *d, unsigned int lim,
					unsigned int nb)
{
	unsigned int	i;

	i = 0;
	while (i < lim)
	{
		pthread_mutex_destroy(&(forks_mutex[i]));
		i++;
	}
	if (nb > 0)
		pthread_mutex_destroy(&(d->print_mutex));
	if (nb > 1)
		pthread_mutex_destroy(&(d->meal_mutex));
	if (nb > 2)
		pthread_mutex_destroy(&(d->died_mutex));
	return (-1);
}

void	init_philosophers(t_philo *philo, t_d *d)
{
	unsigned int	i;

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
		philo[i].bol_thread = 0;
		philo[i].nb_meals = 0;
		philo[i].life = 0;
		i++;
	}
}

int	create_threads(t_philo *philo)
{
	unsigned int	i;

	i = 0;
	while (i < philo->d->n_philo)
	{
		if (pthread_create(&(philo[i].thread), NULL, &routine,
				(void *)(&(philo[i]))) != 0)
			return (-1);
		i += 2;
	}
	usleep(5000);
	i = 1;
	while (i < philo->d->n_philo)
	{
		if (pthread_create(&(philo[i].thread), NULL, &routine,
				(void *)(&(philo[i]))) != 0)
			return (-1);
		i += 2;
	}
	return (0);
}

int	launch_philo_init(t_d *d, t_philo *philo, pthread_mutex_t *forks_mutex)
{
	unsigned int	i;

	i = 0;
	while (i < d->n_philo)
	{
		if (pthread_mutex_init(&(forks_mutex[i]), NULL) != 0)
			return (ft_destroy(forks_mutex, d, i, 0));
		i++;
	}
	d->forks_mutex = forks_mutex;
	if (pthread_mutex_init(&(d->print_mutex), NULL) != 0)
		return (ft_destroy(forks_mutex, d, i, 1));
	if (pthread_mutex_init(&(d->meal_mutex), NULL) != 0)
		return (ft_destroy(forks_mutex, d, i, 2));
	if (pthread_mutex_init(&(d->died_mutex), NULL) != 0)
		return (ft_destroy(forks_mutex, d, i, 3));
	init_philosophers(philo, d);
	gettimeofday(&(d->begin), NULL);
	if (create_threads(philo) == -1)
		return (ft_destroy(forks_mutex, d, i, 3));
	return (0);
}
