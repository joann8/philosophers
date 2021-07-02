/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_test.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jacher <jacher@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/24 15:07:35 by jacher            #+#    #+#             */
/*   Updated: 2021/07/01 14:53:08 by jacher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	ft_free_help(t_philo *philo, pthread_mutex_t *forks_mutex, int res)
{
//	if (forks_table)
//		free(forks_table);
	if (forks_mutex)
		free(forks_mutex);
	if (philo)
		free(philo);
	return (res);
}

void init_philosophers(t_philo *philo, t_d *d, pthread_mutex_t *forks_mutex)
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
	//	philo[i].forks_table = forks_table;
		i++;
	}
}

void update_life(t_philo *philo, time_t init, time_t *life)
{
	struct timeval start;
	
	gettimeofday(&start, NULL);
	if (philo->nb_meals == 0)
		*life = philo->d->time_to_die + init - (start.tv_sec * 1000 + start.tv_usec);
	else
		*life = philo->d->time_to_die
			+ philo->time_last_meal.tv_sec * 1000 + philo->time_last_meal.tv_usec
			- (start.tv_sec * 1000 + start.tv_usec);
}
				
void eat_sleep_think(t_philo *philo, time_t *life, struct timeval start)
{
	print_time_stamp();
	printf("Philo %d took right fork (fork number %d)\n", philo->num, philo->num_fork_r);
	philo->status = EAT;
	print_state(philo->status, philo->num, philo->d->begin);
	philo->nb_meals += 1;
	gettimeofday(&(philo->time_last_meal), NULL);
	*life = philo->d->time_to_die;
	usleep(philo->d->time_to_eat);
	pthread_mutex_unlock(&(philo->forks_mutex[philo->num_fork_l -1]));
//	print_time_stamp();
//	printf("Philo %d put down right left (fork number %d)\n", philo->num, philo->num_fork_l);
	pthread_mutex_unlock(&(philo->forks_mutex[philo->num_fork_r -1]));
//	print_time_stamp();
//	printf("Philo %d put down right fork (fork number %d)\n", philo->num, philo->num_fork_r);
	philo->status = SLEEP;
	print_state(philo->status, philo->num, philo->d->begin);
	usleep(philo->d->time_to_sleep);
	philo->status = THINK;
	print_state(philo->status, philo->num, philo->d->begin);
}

void *routine(void *arg)
{
	t_philo *philo;
	struct timeval start;
	time_t time;
	time_t init;
	time_t life;
	
	philo = (t_philo *)arg;
	gettimeofday(&start, NULL);
	
	init = start.tv_sec * 1000 + start.tv_usec;
	life = philo->d->time_to_die;
	printf("\nINIT - Philo %d - life = %ld\n", philo->num, life);
	print_state(philo->status, philo->num, philo->d->begin);

	while (life > 0 && philo->d->n_philo_alive == philo->d->n_philo)
	{
		printf("\nPHILO %d : philo alive = %d | philo total = %d\n", philo->num, philo->d->n_philo_alive, philo->d->n_philo);
		pthread_mutex_lock(&(philo->forks_mutex[philo->num_fork_l - 1]));
		philo->status = FORK;
		print_state(philo->status, philo->num, philo->d->begin);
		print_time_stamp();
		printf("Philo %d took left fork (fork number %d)\n", philo->num, philo->num_fork_l);
		pthread_mutex_lock(&(philo->forks_mutex[philo->num_fork_r - 1]));
		print_state(philo->status, philo->num, philo->d->begin);
		eat_sleep_think(philo, &life, start);
		update_life(philo, init, &life);
	}
	if (life < 0)
	{
		philo->status = DIED;
		print_state(philo->status, philo->num, philo->d->begin);
		pthread_mutex_lock(philo->d->dead_mutex);
		philo->d->n_philo_alive--;
		pthread_mutex_unlock(philo->d->dead_mutex);
	}
	return ((void*)philo);
}

int create_threads(t_philo *philo, t_d *d, pthread_mutex_t *forks_mutex)
{
	int i;

	i = 0;
	while (i < d->n_philo)
	{
		if (pthread_create(&(philo[i].thread), NULL, &routine, (void *)(&(philo[i]))) != 0)
			return (ft_free_help(philo, forks_mutex, -1));
		i += 2;
	}
	usleep(5000);
	i = 1;
	while (i < d->n_philo)
	{
		if (pthread_create(&(philo[i].thread), NULL, &routine, (void *)(&(philo[i]))) != 0)
			return (ft_free_help(philo, forks_mutex, -1));
		i += 2;
	}
	return (0);
}

int launch_philo(t_d *d)
{
	unsigned int	i;
	t_philo			*philo;
	pthread_mutex_t	*forks_mutex;
	pthread_mutex_t	dead;	
	
	gettimeofday(&(d->begin), NULL);

	pthread_mutex_init(&dead, NULL);
	d->dead_mutex = &dead;
	
	philo = malloc(sizeof(t_philo) * d->n_philo);
	if (philo == NULL)
		return (-1);
	
	forks_mutex = malloc(sizeof(pthread_mutex_t) * d->n_philo);
	if (forks_mutex == NULL)
		return (ft_free_help(philo, NULL, -1));
	i = 0;
	while (i < d->n_philo)
	{
		pthread_mutex_init(&(forks_mutex[i]), NULL);
		i++;
	}
	init_philosophers(philo, d, forks_mutex);
	
	create_threads(philo, d, forks_mutex); 
	
	while (d->n_philo == d->n_philo_alive)
		usleep(100);
	
	i = 0;
	while (i < d->n_philo)
	{
		if (pthread_join(philo[i].thread, NULL) != 0)
			return (ft_free_help(philo, forks_mutex, -1));
		i++;
	}
	i = 0;
	while (i < d->n_philo)
	{
		pthread_mutex_destroy(&(forks_mutex[i]));
		i++;
	}
	pthread_mutex_destroy(&dead);
	
	return (ft_free_help(philo, forks_mutex, 0));
}
