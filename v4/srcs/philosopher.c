/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jacher <jacher@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/24 15:07:35 by jacher            #+#    #+#             */
/*   Updated: 2021/07/04 09:26:54 by jacher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

// A FAIRE : checker mutex unlock + cas 1 philo
// cas 50 1500 200 200 >> des mutex qui trainent


int eat_sleep_think(t_philo *philo)
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
	pthread_mutex_unlock(&(philo->d->forks_mutex[philo->num_fork_l -1]));
	pthread_mutex_unlock(&(philo->d->forks_mutex[philo->num_fork_r -1]));
	usleep(philo->d->time_to_sleep * 1000);
	philo->status = THINK;
	print_state(philo);
	return (0);
}

void *routine(void *arg)
{
	t_philo			*philo;
	
	philo = (t_philo *)arg;
	philo->life = get_time() + philo->d->time_to_die;
	philo->bol_thread = 1;
	print_state(philo);

	while (philo->life > get_time() && check_all_alive(philo, 2) == 0
	&& check_all_eat(philo, 2) == 0)
	{
		pthread_mutex_lock(&(philo->d->check_mutex));
		if (philo->d->forks_table[philo->num_fork_r - 1] == 0)
		{
			pthread_mutex_lock(&(philo->d->forks_mutex[philo->num_fork_l - 1]));
			philo->status = FORK1;
			print_state(philo);
			if (philo->d->n_philo == 1)
			{
				while (get_time() < philo->life)
					usleep(1);
				pthread_mutex_unlock(&(philo->d->forks_mutex[philo->num_fork_l - 1]));
			}
			else
			{
				pthread_mutex_lock(&(philo->d->forks_mutex[philo->num_fork_r - 1]));
				philo->d->forks_table[philo->num_fork_r - 1] = 1;
				philo->status = FORK2;
				pthread_mutex_unlock(&(philo->d->check_mutex));
				print_state(philo);
				eat_sleep_think(philo);
				pthread_mutex_lock(&(philo->d->check_mutex));
				philo->d->forks_table[philo->num_fork_r - 1] = 0;
			}
		}
		pthread_mutex_unlock(&(philo->d->check_mutex));
	}
	return (NULL);
}

int launch_philo(t_d *d)
{
	unsigned int	i;
	t_philo			*philo;
	pthread_mutex_t	*forks_mutex;
	int				*forks_table;
	
	philo = malloc(sizeof(t_philo) * d->n_philo);
	if (philo == NULL)
		return (-1);
	forks_mutex = malloc(sizeof(pthread_mutex_t) * d->n_philo);
	if (forks_mutex == NULL)
		return (ft_free_help(philo, NULL, NULL, -1));
	forks_table = malloc(sizeof(int) * d->n_philo);
	if (forks_table == NULL)
		return (ft_free_help(philo, forks_mutex, NULL, -1));
	
	if (launch_philo_init(d, philo, forks_mutex, forks_table) == -1)
		return (ft_free_help(philo, forks_mutex, forks_table, -1));
	
	if (create_threads(philo, forks_mutex, forks_table) == -1)
		return (ft_destroy(forks_mutex, d, d->n_philo, 4));
	
	while (check_all_alive(philo, 1) == 0 && check_all_eat(philo, 1) == 0)
		usleep(100);
	
	i = 0;
	while (i < d->n_philo)
	{
		if (pthread_join(philo[i].thread, NULL) != 0)
			return (ft_free_help(philo, forks_mutex,forks_table, -1));
		i++;
	}
	if (check_all_eat(philo, 1) == 1)
		printf("All philo ate %d meals\n", d->n_eat);
	ft_destroy(forks_mutex, d, d->n_philo, 4);
	return (ft_free_help(philo, forks_mutex, forks_table, 0));
}
