/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jacher <jacher@student.42.fr               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/24 10:11:05 by jacher            #+#    #+#             */
/*   Updated: 2021/07/02 21:03:31 by jacher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

useconds_t get_time(void)
{
 	struct timeval	tmp;
	useconds_t		res;

	gettimeofday(&tmp, NULL);
	res = tmp.tv_sec * 1000 + tmp.tv_usec / 1000;
	return (res);
}

useconds_t get_dif_stamp(struct timeval start)
{
	 useconds_t res;

	 res = get_time() - (start.tv_sec * 1000 + start.tv_usec / 1000);
	 return (res);
}

void print_dif_stamp(useconds_t time)
{
	 printf("%u ms - ", time);
}
