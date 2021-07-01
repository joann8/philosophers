/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jacher <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/01 15:08:53 by jacher            #+#    #+#             */
/*   Updated: 2021/07/01 19:35:20 by jacher           ###   ########.fr       */
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

int	check_all_alive(t_philo *philo)
{
	unsigned int i;

	i = 0;
	while (i < philo->d->n_philo)
	{
		if (philo[i].bol_death == 1)
			return (1);
		i++;
	}
	return (0);
}
