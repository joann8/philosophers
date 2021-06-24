#include "../philo.h"

void print_data(t_d *d)
{
	printf("d->n_philo = %u\n", d->n_philo);
	printf("d->time_to_die = %u\n", d->time_to_die);
	printf("d->time_to_eat = %u\n", d->time_to_eat);
	printf("d->time_to_sleep = %u\n", d->time_to_sleep);
	printf("bol_eat = %d\n", d->bol_eat);
	if (d->bol_eat == 1)
		printf("d->time_to_sleep = %u\n", d->n_eat);
	return;
}

void print_philo(t_philo philo)
{
	printf("philo.num = %d\n", philo.num);
	printf("philo.status = %d\n", philo.status);
	printf("philo.num_fork_l = %d\n", philo.num_fork_l);
	printf("philo.num_fork_r = %d\n", philo.num_fork_r);
	printf("philo.nb_meals = %d\n", philo.nb_meals);
	printf("philo.time_last_meal = %d\n", philo.time_last_meal);
	printf("philo.bol_death = %d\n", philo.bol_death);
	return;
}
