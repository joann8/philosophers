/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jacher <jacher@student.42.fr               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/24 10:11:05 by jacher            #+#    #+#             */
/*   Updated: 2021/07/04 11:44:14 by jacher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int main(int ac, char **av)
{
	t_d d;
	t_philo			*philo;
	pthread_mutex_t	*forks_mutex;

	if (parsing(ac, av, &d) == -1)
	{
		printf("Error parsing\n");
		return (1);
	}
	philo = malloc(sizeof(t_philo) * d.n_philo);
	if (philo == NULL)
		return (-1);
	d.philo_begin = philo;
	forks_mutex = malloc(sizeof(pthread_mutex_t) * d.n_philo);
	if (forks_mutex == NULL)
		return (ft_free_help(philo, NULL, -1));
	return (launch_philo(&d, philo, forks_mutex));
}
