#ifndef PHILO_H
# define PHILO_H

# include <string.h> 
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>

typedef enum		e_status
{
	THINK,
	FORK,
	EAT,
	SLEEP,
	DIED
}					t_status;

struct s_philo;

typedef struct		s_d
{
	unsigned int	n_philo;
	useconds_t		time_to_die;
	useconds_t		time_to_eat;
	useconds_t		time_to_sleep;
	int				bol_eat;
	unsigned int	n_eat;
	struct timeval	begin;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	died_mutex;
	pthread_mutex_t	meal_mutex;
	pthread_mutex_t	*forks_mutex;
	unsigned int	bol_someone_died;
	struct s_philo	*philo_begin;
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
	unsigned int	nb_meals;
	useconds_t		life;
}					t_philo;
	
int					parsing(int ac, char **av, t_d *d);
void				print_state(t_philo *philo);
useconds_t			get_time(void);
void				print_dif_stamp(useconds_t time);
useconds_t			get_dif_stamp(struct timeval start);
int					launch_philo(t_d *d, t_philo *philo, pthread_mutex_t *forks_mutex);
void				*routine(void *arg);
int					check_all_alive(t_philo *philo, int mod);
int					check_all_eat(t_philo *philo, int mod);
void				philo_dies(t_philo *philo);
int					launch_philo_init(t_d *d, t_philo *philo,
						pthread_mutex_t *forks_mutex);
int					create_threads(t_philo *philo,
						pthread_mutex_t *forks_mutex);
void				init_philosophers(t_philo *philo, t_d *d);
int					ft_destroy(pthread_mutex_t *forks_mutex, t_d *d,
						unsigned int lim, unsigned int nb);
int					ft_free_help(t_philo *philo, pthread_mutex_t *forks_mutex,
						int res);

#endif
