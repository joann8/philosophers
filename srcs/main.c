/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jacher <jacher@student.42.fr               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/24 10:11:05 by jacher            #+#    #+#             */
/*   Updated: 2021/06/24 19:02:32 by jacher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void print_time_stamp(void)
{
 struct timeval start;

 gettimeofday(&start, NULL);
 printf("%ld ms - ", start.tv_sec * 1000 + start.tv_usec);
}

int main(int ac, char **av)
{
	t_d d;

	print_time_stamp();
	if (parsing(ac, av, &d) == -1)
	{
		printf("Error parsing\n");
		return (1);
	}
	else
	{
		printf("parsing OK\n");
		print_data(&d);
	}
	launch_philo(&d);
	print_time_stamp();
	return (0);
}
