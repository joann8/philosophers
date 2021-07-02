/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jacher <jacher@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/24 15:07:35 by jacher            #+#    #+#             */
/*   Updated: 2021/07/02 21:17:30 by jacher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

// A FAIRE : checker mutex unlock + cas 1 philo
// cas 50 1500 200 200 >> des mutex qui trainent

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

void init_philosophers(t_philo *philo, t_d *d)
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
		philo[i].num_forks = 0;
		philo[i].bol_thread = 0;
		philo[i].nb_meals = 0;
		i++;
	}
}

int eat_sleep_think(t_philo *philo)
{
	philo->status = EAT;
	print_state(philo);
	pthread_mutex_lock(&(philo->d->meal_mutex));
	philo->nb_meals += 1;
	philo->life = get_time() +  philo->d->time_to_die;
	pthread_mutex_unlock(&(philo->d->meal_mutex));
	usleep(philo->d->time_to_eat * 1000);
	pthread_mutex_unlock(&(philo->d->forks_mutex[philo->num_fork_l -1]));
	pthread_mutex_unlock(&(philo->d->forks_mutex[philo->num_fork_r -1]));
	philo->status = SLEEP;
	print_state(philo);
	if ( get_time() + philo->d->time_to_sleep < philo->life)
	{
		philo_dies(philo);
		return (1);
	}
	else
	{
		usleep(philo->d->time_to_sleep * 1000);
		philo->status = THINK;
		print_state(philo);
	}
	return (0);
}

void *routine(void *arg)
{
	t_philo			*philo;
	
	philo = (t_philo *)arg;
	philo->life = get_time() + philo->d->time_to_die;
	philo->bol_thread = 1;
	print_state(philo);

	while(philo->d->bol_someone_died == 0
	&& philo->life > get_time()
	&& (philo->d->bol_eat == 0
	|| (philo->d->bol_eat == 1 && philo->nb_meals < philo->d->n_eat)))
	{
		if (get_time() > philo->life)
		{
			philo_dies(philo);
			break ;
		}
		pthread_mutex_lock(&(philo->d->check_mutex));
		if (philo->d->forks_table[philo->num_fork_r - 1] == 0)
		{
			pthread_mutex_lock(&(philo->d->forks_mutex[philo->num_fork_l - 1]));
			if (get_time() < philo->life)
			{
				philo->status = FORK;
				print_state(philo);
			}
			else
			{
				philo_dies(philo);
				pthread_mutex_unlock(&(philo->d->forks_mutex[philo->num_fork_l - 1]));
				pthread_mutex_unlock(&(philo->d->check_mutex));
				break ;
			}
			pthread_mutex_lock(&(philo->d->forks_mutex[philo->num_fork_r - 1]));
			philo->d->forks_table[philo->num_fork_r - 1] = 1;
			pthread_mutex_unlock(&(philo->d->check_mutex));
			if ( get_time() < philo->life)
			{
				print_state(philo);
				if (eat_sleep_think(philo) == 1)
					break;
			}
			else
			{	
				philo_dies(philo);
				pthread_mutex_unlock(&(philo->d->forks_mutex[philo->num_fork_l - 1]));
				pthread_mutex_unlock(&(philo->d->forks_mutex[philo->num_fork_r - 1]));
				break ;
			}
			pthread_mutex_lock(&(philo->d->check_mutex));
			philo->d->forks_table[philo->num_fork_r - 1] = 0;
		}
		pthread_mutex_unlock(&(philo->d->check_mutex));
		if (get_time() > philo->life)
		{
			philo_dies(philo);
			break ;
		}
	}
	return (NULL);
}

int create_threads(t_philo *philo, pthread_mutex_t *forks_mutex, int *forks_table)
{
	int i;

	i = 0;
	while (i < philo->d->n_philo)
	{
		if (pthread_create(&(philo[i].thread), NULL, &routine, (void *)(&(philo[i]))) != 0)
			return (ft_free_help(philo, forks_mutex, forks_table, 1));
		i += 2;
	}
	usleep(5000);
	i = 1;
	while (i < philo->d->n_philo)
	{
		if (pthread_create(&(philo[i].thread), NULL, &routine, (void *)(&(philo[i]))) != 0)
			return (ft_free_help(philo, forks_mutex, forks_table, -1));
		i += 2;
	}
	return (0);
}

int launch_philo(t_d *d)
{
	unsigned int	i;
	t_philo			*philo;
	pthread_mutex_t	*forks_mutex;
	int				*forks_table;
	
	gettimeofday(&(d->begin), NULL);

	philo = malloc(sizeof(t_philo) * d->n_philo);
	if (philo == NULL)
		return (-1);
	
	forks_table = malloc(sizeof(int) * d->n_philo);
	if (forks_table == NULL)
		return (ft_free_help(philo, NULL, NULL, -1)); //fork table a ajouter
	
	forks_mutex = malloc(sizeof(pthread_mutex_t) * d->n_philo);
	if (forks_mutex == NULL)
		return (ft_free_help(philo, NULL, forks_table, -1));
	i = 0;
	while (i < d->n_philo)
	{
		pthread_mutex_init(&(forks_mutex[i]), NULL);
		forks_table[i] = 0;
		i++;
	}
	d->forks_mutex = forks_mutex;
	d->forks_table = forks_table;
	pthread_mutex_init(&(d->print_mutex), NULL);
	pthread_mutex_init(&(d->check_mutex), NULL);
	pthread_mutex_init(&(d->meal_mutex), NULL);
	pthread_mutex_init(&(d->died_mutex), NULL);

	init_philosophers(philo, d);
	if (create_threads(philo, forks_mutex, forks_table) == -1)
		return (-1);
	
	while (d->bol_someone_died == 0
	&& check_all_alive(philo) == 0
	&& (d->bol_eat == 0  
	|| (d->bol_eat == 1 && check_all_eat(philo) == 0)))
		usleep(100);
	
	i = 0;
	while (i < d->n_philo)
	{
		if (pthread_join(philo[i].thread, NULL) != 0)
			return (ft_free_help(philo, forks_mutex,forks_table, -1));
		i++;
	}
	if (d->bol_eat == 1 && check_all_eat(philo) == 1)
		printf("All philo ate %d meals\n", d->n_eat);
	
	i = 0;
	while (i < d->n_philo)
	{
		pthread_mutex_destroy(&(forks_mutex[i]));
		i++;
	}
	pthread_mutex_destroy(&(d->print_mutex));
	pthread_mutex_destroy(&(d->check_mutex));
	pthread_mutex_destroy(&(d->meal_mutex));
	pthread_mutex_destroy(&(d->died_mutex));
	return (ft_free_help(philo, forks_mutex, forks_table,0));
}
