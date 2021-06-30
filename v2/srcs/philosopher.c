/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jacher <jacher@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/24 15:07:35 by jacher            #+#    #+#             */
/*   Updated: 2021/06/30 13:59:08 by jacher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	ft_free_help(t_philo *philo, pthread_mutex_t *forks_mutex, int *forks_table, int res)
{
	if (forks_table)
		free(forks_table);
	if (forks_mutex)
		free(forks_mutex);
	if (philo)
		free(philo);
	return (res);
}

void init_philosophers(t_philo *philo, t_d *d, pthread_mutex_t *forks_mutex, int *forks_table)
{
	int i;

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
		philo[i].nb_meals = 0;
		philo[i].bol_death = 0;
		philo[i].forks_mutex = forks_mutex;
		philo[i].forks_table = forks_table;
		i++;
	}
}

void *routine(void *arg)
{
	t_philo *philo;
	struct timeval start;
	time_t time;
	time_t init;
	time_t life;
	
	philo = (t_philo *)arg;
	print_state(philo->status, philo->num);
	
	gettimeofday(&start, NULL);
	init = start.tv_sec * 1000 + start.tv_usec;
	life = philo->d->time_to_die;
	printf("INIT - Philo %d - life = %ld\n", philo->num, life);

	while (life > 0 && philo->d->n_philo_alive == philo->d->n_philo)
	{
		while ((philo->status == THINK || philo->status == FORK)
				&& life >= 0
				&& philo->d->n_philo_alive == philo->d->n_philo)
		{
			if (philo->forks_table[philo->num_fork_l -1] == 0)
			{
				philo->forks_table[philo->num_fork_l - 1] = 1;
				philo->status = FORK;
				print_state(philo->status, philo->num);
				pthread_mutex_lock(&(philo->forks_mutex[philo->num_fork_l - 1]));
				printf("Philo %d took left fork | forks_table[%d] = %d\n", philo->num, philo->num_fork_l - 1, philo->forks_table[philo->num_fork_l -1]);
				while (life >= 0 && philo->d->n_philo_alive == philo->d->n_philo)
				{
					print_time_stamp();
					printf("Philo %d - life = %ld\n", philo->num, life);
					if (philo->forks_table[philo->num_fork_r -1] == 0)
					{
						philo->forks_table[philo->num_fork_r - 1] = 1;
						pthread_mutex_lock(&(philo->forks_mutex[philo->num_fork_r - 1]));
						philo->status = EAT;
						print_state(philo->status, philo->num);
						printf("Philo %d took right fork | forks_table[%d] = %d\n", philo->num, philo->num_fork_r - 1, philo->forks_table[philo->num_fork_r -1]);
						philo->nb_meals += 1;
						gettimeofday(&(philo->time_last_meal), NULL);
						life = philo->d->time_to_die;
						usleep(philo->d->time_to_eat);
						pthread_mutex_unlock(&(philo->forks_mutex[philo->num_fork_l -1]));
						philo->forks_table[philo->num_fork_l - 1] = 0;
						print_time_stamp();
						printf("Philo %d put down left fork | forks_table[%d] = %d\n", philo->num, philo->num_fork_l - 1, philo->forks_table[philo->num_fork_l -1]);
						pthread_mutex_unlock(&(philo->forks_mutex[philo->num_fork_r -1]));
						philo->forks_table[philo->num_fork_r - 1] = 0;
						print_time_stamp();
						printf("Philo %d put down right fork | forks_table[%d] = %d\n", philo->num, philo->num_fork_r - 1, philo->forks_table[philo->num_fork_r -1]);
						philo->status = SLEEP;
						break ;
					}
					else
					{
						gettimeofday(&start, NULL);
						time = start.tv_sec *1000 + start.tv_usec;
						if (philo->nb_meals == 0)
							life = philo->d->time_to_die + init - time;
						else
							life = philo->d->time_to_die + philo->time_last_meal.tv_sec * 1000 + philo->time_last_meal.tv_usec - time;
					}
				}
			}
			else
			{
				gettimeofday(&start, NULL);
				time = start.tv_sec *1000 + start.tv_usec;
				if (philo->nb_meals == 0)
					life = init + philo->d->time_to_die - time;
				else
					life = philo->d->time_to_die + philo->time_last_meal.tv_sec * 1000 + philo->time_last_meal.tv_usec - time;
			}
		}
		if (life < 0)
		{
			philo->status = DIED;
			print_state(philo->status, philo->num);
			philo->d->n_philo_alive--;
			break ;
		}
		if (philo->status == SLEEP)
		{
			print_state(philo->status, philo->num);
			usleep(philo->d->time_to_sleep);
			philo->status = THINK;
			print_state(philo->status, philo->num);
		}
		gettimeofday(&start, NULL);
		time = start.tv_sec *1000 + start.tv_usec;
		life = philo->d->time_to_die + philo->time_last_meal.tv_sec * 1000 + philo->time_last_meal.tv_usec - time;
	}
	/*printf("*****\nOne philosophe has been created / done routine\n");
	print_philo(*philo);
	if (philo
	printf("************\n");*/
	return ((void*)philo);
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
	i = 0;
	while (i < d->n_philo)
	{
		forks_table[i] = 0;
		i++;
	}
	init_philosophers(philo, d, forks_mutex, forks_table);
	i = 0;
	while (i < d->n_philo)
	{
		if (pthread_create(&(philo[i].thread), NULL, &routine, (void *)(&(philo[i]))) != 0)
			return (ft_free_help(philo, forks_mutex, forks_table, -1));
		i++;
	}
	i = 0;
	while (i < d->n_philo)
	{
		if (pthread_join(philo[i].thread, NULL) != 0)
			return (ft_free_help(philo, forks_mutex, forks_table, -1));
		i++;
	}
	return (ft_free_help(philo, forks_mutex, forks_table, 0));
}
		

