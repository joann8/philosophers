/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jacher <jacher@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/04 10:07:46 by jacher            #+#    #+#             */
/*   Updated: 2021/07/04 10:46:12 by jacher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void print_state(t_philo *philo)
{
	pthread_mutex_lock(&(philo->d->print_mutex));
	pthread_mutex_lock(&(philo->d->died_mutex));
	if (philo->d->bol_someone_died == 0)
	{
		print_dif_stamp(get_dif_stamp(philo->d->begin));
		if (philo->status == EAT)
			printf("%d is eating\n", philo->num);
		else if (philo->status == FORK)
			printf("%d has taken a fork\n", philo->num);
		else if (philo->status == SLEEP)
			printf("%d is sleeping\n", philo->num);
		else if (philo->status == THINK)
			printf("%d is thinking\n", philo->num);
		else
			printf("%d died\n", philo->num);
	}
	pthread_mutex_unlock(&(philo->d->print_mutex));
	pthread_mutex_unlock(&(philo->d->died_mutex));
}
