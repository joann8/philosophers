#include "../philo.h"

void print_data(t_d *d)
{
	printf("d->n_philo = %u\n", d->n_philo);
	printf("d->time_to_die = %u\n", d->time_to_die);
	printf("d->time_to_eat = %u\n", d->time_to_eat);
	printf("d->time_to_sleep = %u\n", d->time_to_sleep);
	printf("bol_eat = %d\n", d->bol_eat);
	if (d->bol_eat == 1)
		printf("d->n_eat = %u\n", d->n_eat);
	return;
}

void print_philo(t_philo philo)
{
	printf("philo.num = %d\n", philo.num);
	printf("philo.status = %d\n", philo.status);
	printf("philo.num_fork_l = %d\n", philo.num_fork_l);
	printf("philo.num_fork_r = %d\n", philo.num_fork_r);
	printf("philo.nb_meals = %d\n", philo.nb_meals);
	printf("philo.bol_death = %d\n", philo.bol_death);
	return;
}

void print_state(t_status status, int num, struct timeval start)
{
//	print_time_stamp();
	print_dif_stamp(get_dif_stamp(start));
	if (status == EAT)
		printf("%d is eating\n", num);
	else if (status == FORK)
		printf("%d has taken a fork\n", num);
	else if (status == SLEEP)
		printf("%d is sleeping\n", num);
	else if (status == THINK)
		printf("%d is thinking\n", num);
	else if (status == DIED)
		printf("%d died\n", num);
}
