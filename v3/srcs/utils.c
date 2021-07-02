/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jacher <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/01 15:08:53 by jacher            #+#    #+#             */
/*   Updated: 2021/07/02 20:57:54 by jacher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	check_all_eat(t_philo *philo)
{
	unsigned int i;

	i = 0;
	if (philo->d->bol_eat == 1)
	{
		i = 0;
		while (i < philo->d->n_philo)
		{
			if (philo[i].nb_meals != philo->d->n_eat)
				return (0);
			i++;
		}
	}
	return (1);
}

void philo_dies(t_philo *philo)
{
	philo->status = DIED;
//	philo->bol_death = 1;
	while (get_time() < philo->life)
		usleep(1);
	print_state(philo);
	pthread_mutex_lock(&(philo->d->died_mutex));
	philo->d->bol_someone_died = 1;
	pthread_mutex_unlock(&(philo->d->died_mutex));
}

int	check_all_alive(t_philo *philo)
{
	unsigned int i;

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
	return (0);
}
