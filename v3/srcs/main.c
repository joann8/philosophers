/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jacher <jacher@student.42.fr               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/24 10:11:05 by jacher            #+#    #+#             */
/*   Updated: 2021/07/02 21:07:38 by jacher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int main(int ac, char **av)
{
	t_d d;

	if (parsing(ac, av, &d) == -1)
	{
		printf("Error parsing\n");
		return (1);
	}
/*	else
	{
		printf("parsing OK\n");
		//print_data(&d);
	}*/
	launch_philo(&d);
	return (0);
}
