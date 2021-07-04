/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jacher <jacher@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/24 15:07:35 by jacher            #+#    #+#             */
/*   Updated: 2021/07/04 11:13:28 by jacher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	eat_sleep_think(t_philo *philo)
{
	philo->status = EAT;
	print_state(philo);
	pthread_mutex_lock(&(philo->d->meal_mutex));
	philo->nb_meals += 1;
	philo->life = get_time() + philo->d->time_to_die;
	pthread_mutex_unlock(&(philo->d->meal_mutex));
	usleep(philo->d->time_to_eat * 1000);
	philo->status = SLEEP;
	print_state(philo);
	pthread_mutex_unlock(&(philo->d->forks_mutex[philo->num_fork_r -1]));
	pthread_mutex_unlock(&(philo->d->forks_mutex[philo->num_fork_l -1]));
	usleep(philo->d->time_to_sleep * 1000);
	philo->status = THINK;
	print_state(philo);
}

void	*routine(void *arg)
{
	t_philo			*philo;

	philo = (t_philo *)arg;
	philo->life = get_time() + philo->d->time_to_die;
	philo->bol_thread = 1;
	print_state(philo);
	while (check_all_alive(philo, 2) == 0 && check_all_eat(philo, 2) == 0)
	{
		pthread_mutex_lock(&(philo->d->forks_mutex[philo->num_fork_l - 1]));
		philo->status = FORK;
		print_state(philo);
		if (philo->d->n_philo == 1)
		{
			pthread_mutex_unlock(&(philo->d->forks_mutex[0]));
			philo_dies(philo);
		}
		else
		{
			pthread_mutex_lock(&(philo->d->forks_mutex[philo->num_fork_r - 1]));
			print_state(philo);
			eat_sleep_think(philo);
		}
	}
	return (NULL);
}

int		launch_philo(t_d *d, t_philo *philo, pthread_mutex_t *forks_mutex)
{
	unsigned int	i;
	
	if (launch_philo_init(d, philo, forks_mutex) == -1)
		return (ft_free_help(philo, forks_mutex, 1));
	while (check_all_alive(philo, 1) == 0 && check_all_eat(philo, 1) == 0)
		usleep(100);
	i = 0;
	while (i < d->n_philo)
	{
		if (pthread_join(philo[i].thread, NULL) != 0)
		{
			ft_destroy(forks_mutex, d, d->n_philo, 3);
			return (ft_free_help(philo, forks_mutex, 1));
		}
		i++;
	}
	if (d->bol_someone_died == 0 && check_all_eat(philo, 1) == 1)
		printf("All philosophers ate %d meals\n", d->n_eat);
	ft_destroy(forks_mutex, d, d->n_philo, 3);
	return (ft_free_help(philo, forks_mutex, 0));
}
