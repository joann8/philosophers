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
	FORK,
	EAT,
	SLEEP,
	DIED
}					t_status;

typedef struct		s_d
{
	unsigned int	n_philo;
	unsigned int	n_philo_alive;
	unsigned int	time_to_die;
	unsigned int	time_to_eat;
	unsigned int	time_to_sleep;
	int				bol_eat;
	unsigned int	n_eat;
	pthread_t		*philosophe;
	pthread_mutex_t	*dead_mutex;
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
	pthread_mutex_t	*forks_mutex;
	int				*forks_table;
	int				nb_meals;
	struct timeval	time_last_meal;
	int				bol_death;
}					t_philo;
	
int	parsing(int ac, char **av, t_d *d);
void print_data(t_d *d);
void print_philo(t_philo philo);

//time
void print_time_stamp(void);
void print_dif_stamp(long int time);
long int get_dif_stamp(struct timeval start);


void print_state(t_status status, int num, struct timeval start);
int	 launch_philo(t_d *d);

#endif
