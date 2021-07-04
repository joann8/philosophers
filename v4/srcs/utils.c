/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jacher <jacher@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/01 15:08:53 by jacher            #+#    #+#             */
/*   Updated: 2021/07/04 09:14:31 by jacher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	check_all_eat(t_philo *philo, int mod)
{
	unsigned int i;
	unsigned int res;

	i = 0;
	res = 0;
	if (philo->d->bol_eat == 1)
	{
		pthread_mutex_lock(&(philo->d->meal_mutex));
		if (mod == 1)
		{
			i = 0;
			while (i < philo->d->n_philo)
			{
				if (philo[i].nb_meals != philo->d->n_eat)
					break; 
				i++;
			}
			res = 1;
		}
		else if (mod == 2 && philo->nb_meals == philo->d->n_eat)
			res = 1;
		pthread_mutex_unlock(&(philo->d->meal_mutex));
	}
	return (res);
}

void philo_dies(t_philo *philo)
{
	philo->status = DIED;
	while (get_time() < philo->life)
		usleep(1);
	print_state(philo);
	pthread_mutex_lock(&(philo->d->died_mutex));
	philo->d->bol_someone_died = 1;
	pthread_mutex_unlock(&(philo->d->died_mutex));
}

int	check_all_forks(t_philo *philo)
{
	unsigned int i;
	unsigned int count;

	i = 0;
	count = 0;
	pthread_mutex_lock(&(philo->d->check_mutex));
	while (i < philo->d->n_philo)
	{
		if (philo->d->forks_table[i] == 1)
			count++;
		i++;
	}
	pthread_mutex_unlock(&(philo->d->check_mutex));
	if (count == philo->d->n_philo - 1)
		return (1);
	return (0);
}

int	check_all_alive(t_philo *philo, int mod)
{
	unsigned int i;

	pthread_mutex_lock(&(philo->d->died_mutex));
	if (philo->d->bol_someone_died == 1)
	{
		pthread_mutex_unlock(&(philo->d->died_mutex));
		return (1);
	}
	pthread_mutex_unlock(&(philo->d->died_mutex));
	if (mod == 1)
	{
		i = 0;
		while (i < philo->d->n_philo)
		{
			if (philo[i].bol_thread == 1 && philo[i].life < get_time())
			{
				philo_dies(&(philo[i]));
				return (1);
			}
			i++;
		}
	}
	return (0);
}
