/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_tracking.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jacher <jacher@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/01 15:08:53 by jacher            #+#    #+#             */
/*   Updated: 2021/07/05 11:36:04 by jacher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	check_all_eat(t_philo *philo, int mod)
{
	unsigned int	i;
	unsigned int	res;

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
					break ;
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

void	philo_dies(t_philo *philo)
{
	philo->status = DIED;
	while (get_time() <= philo->life)//<
		usleep(1);
	print_state(philo);
	pthread_mutex_lock(&(philo->d->died_mutex));
	philo->d->bol_someone_died = 1;
	pthread_mutex_unlock(&(philo->d->died_mutex));
}

int	check_all_alive_help(t_philo *philo)
{
	unsigned int	i;

	i = 0;
	while (i < philo->d->n_philo)
	{
		if (philo->d->philo_begin[i].bol_thread == 1
			&& (philo->d->bol_eat == 0
				|| philo->d->philo_begin[i].nb_meals != philo->d->n_eat)
			&& philo->d->philo_begin[i].life < get_time())
		{
			philo_dies(&(philo->d->philo_begin[i]));
			pthread_mutex_unlock(&(philo->d->meal_mutex));
			return (1);
		}
		i++;
	}
	pthread_mutex_unlock(&(philo->d->meal_mutex));
	return (0);
}

int	check_all_alive(t_philo *philo, int mod)
{
	pthread_mutex_lock(&(philo->d->died_mutex));
	if (philo->d->bol_someone_died == 1)
	{
		pthread_mutex_unlock(&(philo->d->died_mutex));
		return (1);
	}
	pthread_mutex_unlock(&(philo->d->died_mutex));
	pthread_mutex_lock(&(philo->d->meal_mutex));
	if (mod == 1)
		return (check_all_alive_help(philo));
	else if (mod == 2
		&& (philo->d->bol_eat == 0 || philo->nb_meals != philo->d->n_eat)
		&& philo->life < get_time())
	{
		philo_dies(philo);
		pthread_mutex_unlock(&(philo->d->meal_mutex));
		return (1);
	}
	pthread_mutex_unlock(&(philo->d->meal_mutex));
	return (0);
}
