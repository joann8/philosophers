#ifndef PHILO_H
# define PHILO_H

# include <string.h> //memset
# include <stdio.h> //printf
# include <stdlib.h> //free
# include <unistd.h> // write, usleep
# include <sys/time.h> //gettimeofday
# include <pthread.h>


typedef enum		e_status
{
	THINK,
	EAT,
	SLEEP
}					t_status;

typedef struct		s_d
{
	unsigned int	n_philo;
	unsigned int	time_to_die;
	unsigned int	time_to_eat;
	unsigned int	time_to_sleep;
	int				bol_eat;
	unsigned int	n_eat;
	pthread_t		*philosophe;
}					t_d;

typedef struct		s_philo
{
	pthread_t		thread;
	unsigned int	num;
	t_status		status;
	t_d				*d;
	int 			bol_fork_l;
	unsigned int	num_fork_l;
	unsigned int	num_fork_r;
	int 			bol_fork_r;
	pthread_mutex_t	*forks;
	int				nb_meals;
	int				time_last_meal;
	int				bol_death;
}					t_philo;
	
int	parsing(int ac, char **av, t_d *d);
void print_data(t_d *d);
void print_philo(t_philo philo);
void print_time_stamp(void);
int	 launch_philo(t_d *d);

#endif
