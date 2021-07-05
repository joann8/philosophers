/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jacher <jacher@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/24 10:27:26 by jacher            #+#    #+#             */
/*   Updated: 2021/07/05 11:02:49 by jacher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	ft_atoi_philo(char *str)
{
	long unsigned int		i;
	long unsigned int		res;

	i = 0;
	res = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == ' ')
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			return (-1);
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = res * 10 + str[i] - 48;
		i++;
	}
	if (str[i] != '\0')
		return (-1);
	return (res);
}

int	parsing(int ac, char **av, t_d *d)
{
	if (ac < 5 || ac > 6)
		return (-1);
	if (ft_atoi_philo(av[1]) == -1 || ft_atoi_philo(av[2]) == -1
		|| ft_atoi_philo(av[3]) == -1 || ft_atoi_philo(av[4]) == -1)
		return (-1);
	d->n_philo = (unsigned int)ft_atoi_philo(av[1]);
	d->time_to_die = (useconds_t)ft_atoi_philo(av[2]);
	d->time_to_eat = (useconds_t)ft_atoi_philo(av[3]);
	d->time_to_sleep = (useconds_t)ft_atoi_philo(av[4]);
	d->bol_someone_died = 0;
	if (ac == 6)
	{
		d->bol_eat = 1;
		if (ft_atoi_philo(av[5]) == -1)
			return (-1);
		d->n_eat = (unsigned int)ft_atoi_philo(av[5]);
	}
	else
		d->bol_eat = 0;
	return (0);
}
