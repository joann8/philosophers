/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jacher <jacher@student.42.fr               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/24 10:11:05 by jacher            #+#    #+#             */
/*   Updated: 2021/06/30 15:30:46 by jacher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

long int get_dif_stamp(struct timeval start)
{
	 struct timeval tmp;
	 long int		res;

	 gettimeofday(&tmp, NULL);
	 res = (tmp.tv_sec * 1000 + tmp.tv_usec) - (start.tv_sec * 1000 + start.tv_usec);
	 return (res);
}

void print_dif_stamp(long int time)
{
	 printf("Dif = %ld ms - ", time);
}

void print_time_stamp(void)
{
 	struct timeval start;

	 gettimeofday(&start, NULL);
	 printf("%ld ms - ", start.tv_sec * 1000 + start.tv_usec);
}
