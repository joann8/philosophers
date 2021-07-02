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
	useconds_t		time_to_die;
	useconds_t		time_to_eat;
	useconds_t		time_to_sleep;
	int				bol_eat;
	unsigned int	n_eat;
	struct timeval	begin; //start the clock
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	check_mutex;
	pthread_mutex_t	died_mutex;
	pthread_mutex_t	meal_mutex;
	pthread_mutex_t	*forks_mutex;
	int				*forks_table;
	unsigned int	bol_someone_died;
}					t_d;

typedef struct		s_philo
{
	pthread_t		thread;
	unsigned int	bol_thread;
	unsigned int	num;
	t_status		status;
	t_d				*d;
	unsigned int	num_fork_l;
	unsigned int	num_fork_r;
	unsigned int	num_forks;
	unsigned int	nb_meals;
	useconds_t		life;
}					t_philo;
	
int	parsing(int ac, char **av, t_d *d);
void print_data(t_d *d);
void print_philo(t_philo philo);

//time
useconds_t get_time(void);
void print_dif_stamp(useconds_t time);
useconds_t get_dif_stamp(struct timeval start);


void print_state(t_philo *philo);
int	 launch_philo(t_d *d);

int check_all_alive(t_philo *philo);
int check_all_eat(t_philo *philo);
void philo_dies(t_philo *philo);
#endif
