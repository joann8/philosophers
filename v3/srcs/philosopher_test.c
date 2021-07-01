/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_test.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jacher <jacher@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/24 15:07:35 by jacher            #+#    #+#             */
/*   Updated: 2021/07/01 22:32:16 by jacher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	ft_free_help(t_philo *philo, pthread_mutex_t *forks_mutex, int res)
{
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
		philo[i].nb_meals = 0;
		philo[i].bol_death = 0;
		i++;
	}
}

int check_life(t_philo *philo)
{
	struct timeval	tmp;
	useconds_t		current_life;

	gettimeofday(&tmp, NULL);
	printf("\n**** philo->num = %d ****", philo->num);
	printf("\n**** philo->d->time_to_die = %u ****", philo->d->time_to_die);
	printf("\n**** philo->init = %ld ****", philo->init.tv_sec * 1000 + philo->init.tv_usec / 1000);
	printf("\n**** tmp = %ld ****", tmp.tv_sec * 1000 + tmp.tv_usec / 1000);
	if (philo->nb_meals == 0)
	{
		printf("\n**** philo->init = %ld ****", philo->init.tv_sec * 1000 + philo->init.tv_usec / 1000);
		current_life = philo->d->time_to_die 
		+ (philo->init.tv_sec * 1000 + philo->init.tv_usec / 1000)
		- (tmp.tv_sec * 1000 + tmp.tv_usec / 1000);
	}
	else
	{
		printf("\n**** philo->time_last_meal = %ld ****", philo->time_last_meal.tv_sec * 1000 + philo->time_last_meal.tv_usec / 1000);
		current_life = philo->d->time_to_die 
		+ (philo->time_last_meal.tv_sec * 1000 + philo->time_last_meal.tv_usec / 1000)
		- (tmp.tv_sec * 1000 + tmp.tv_usec / 1000);
	}
	printf("\n**** current = %u ****\n", current_life);
	if (current_life <= 0)
	{
		printf("\n**** RETURN 1****\n");
		return (1);
	}
	return (0);
}
				
void eat_sleep_think(t_philo *philo)
{
	philo->status = EAT;
	print_state(philo);
	pthread_mutex_lock(philo->d->meal_mutex);
	philo->nb_meals += 1;
	printf("---philo %d - nb_meal = %d\n", philo->num, philo->nb_meals);
	gettimeofday(&(philo->time_last_meal), NULL);
	philo->life = get_time() +  philo->d->time_to_die;
	pthread_mutex_unlock(philo->d->meal_mutex);
	usleep(philo->d->time_to_eat * 1000);
	pthread_mutex_unlock(&(philo->d->forks_mutex[philo->num_fork_l -1]));
	pthread_mutex_unlock(&(philo->d->forks_mutex[philo->num_fork_r -1]));
	if ( get_time() + philo->d->time_to_sleep < philo->life)
	{
		philo->status = SLEEP;
		print_state(philo);
		usleep(philo->d->time_to_sleep * 1000);
		philo->status = THINK;
		print_state(philo);
	}
	else
	{
		philo->status = DIED;
		print_state(philo);
		philo->bol_death = 1;
	}
}

void *routine(void *arg)
{
	t_philo			*philo;
	
	philo = (t_philo *)arg;
	printf("\n**** new routine %d ****\n", philo->num);
	gettimeofday(&(philo->init), NULL);
	philo->life = get_time() + philo->d->time_to_die;
	print_state(philo);

	while ((philo->d->bol_eat == 0
	|| (philo->d->bol_eat == 1 && philo->nb_meals < philo->d->n_eat))
	&& philo->life > get_time()
	&& check_all_alive(philo) == 0)

	{
		printf("Philo %d - wait for 1st mutex\n", philo->num);
		pthread_mutex_lock(philo->d->check_mutex);
		if (philo->d->forks_table[philo->num_fork_r - 1] == 0)
		{
			printf("Philo %d - wait for 2nd mutex\n", philo->num);
			pthread_mutex_lock(&(philo->d->forks_mutex[philo->num_fork_l - 1]));
			if ( get_time() < philo->life)
			{
				philo->status = FORK;
				print_state(philo);
			}
			else
			{
				philo->status = DIED;
				print_state(philo);
				philo->bol_death = 1;
			}
			pthread_mutex_lock(&(philo->d->forks_mutex[philo->num_fork_r - 1]));
			philo->d->forks_table[philo->num_fork_r - 1] = 1;
			pthread_mutex_unlock(philo->d->check_mutex);
			if ( get_time() < philo->life)
			{
				print_state(philo);
				eat_sleep_think(philo);
			}
			else
			{	
				if (philo->status != DIED)
				{
					philo->status = DIED;
					print_state(philo);
					philo->bol_death = 1;
				}
				pthread_mutex_unlock(&(philo->d->forks_mutex[philo->num_fork_l -1]));
				pthread_mutex_unlock(&(philo->d->forks_mutex[philo->num_fork_r -1]));
			}
			pthread_mutex_lock(philo->d->check_mutex);
			philo->d->forks_table[philo->num_fork_r - 1] = 0;
		}
		pthread_mutex_unlock(philo->d->check_mutex);
	}
	if ((philo->d->bol_eat == 1 && philo->nb_meals ==  philo->d->n_eat))
	{
		printf("!!! philo %d eat enough\n", philo->num);
	}
	//else if (check_life(philo) == 0) philo->life > get_time())
/*	else if (philo->life < get_time())
	{
		philo->status = DIED;
		print_state(philo);
		philo->bol_death = 1;
	}*/
	return (NULL);
}

int create_threads(t_philo *philo, pthread_mutex_t *forks_mutex)
{
	int i;

	i = 0;
	while (i < philo->d->n_philo)
	{
		if (pthread_create(&(philo[i].thread), NULL, &routine, (void *)(&(philo[i]))) != 0)
			return (ft_free_help(philo, forks_mutex, -1));
		i += 2;
	}
	usleep(5000);
	i = 1;
	while (i < philo->d->n_philo)
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
	int				*forks_table;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	check_mutex;
	pthread_mutex_t	meal_mutex;
	
	gettimeofday(&(d->begin), NULL);

	philo = malloc(sizeof(t_philo) * d->n_philo);
	if (philo == NULL)
		return (-1);
	
	forks_table = malloc(sizeof(int) * d->n_philo);
	if (forks_table == NULL)
		return (ft_free_help(philo, NULL, -1)); //fork table a ajouter
	
	forks_mutex = malloc(sizeof(pthread_mutex_t) * d->n_philo);
	if (forks_mutex == NULL)
		return (ft_free_help(philo, NULL, -1));
	i = 0;
	while (i < d->n_philo)
	{
		pthread_mutex_init(&(forks_mutex[i]), NULL);
		forks_table[i] = 0;
		i++;
	}
	d->forks_mutex = forks_mutex;
	d->forks_table = forks_table;
	pthread_mutex_init(&print_mutex, NULL);
	pthread_mutex_init(&check_mutex, NULL);
	pthread_mutex_init(&meal_mutex, NULL);
	d->print_mutex = &print_mutex;
	d->check_mutex = &check_mutex;
	d->meal_mutex = &meal_mutex;

	init_philosophers(philo, d);
	if (create_threads(philo, forks_mutex) == -1)
		return (-1);
	
	while (check_all_alive(philo) == 0
	&& (d->bol_eat == 0  
	|| (d->bol_eat == 1 && check_all_eat(philo) == 0)))
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
	
	pthread_mutex_destroy(&print_mutex);
	return (ft_free_help(philo, forks_mutex, 0));
}
