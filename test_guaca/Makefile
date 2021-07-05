# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jacher <jacher@student.42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/11/11 09:41:42 by jacher            #+#    #+#              #
#    Updated: 2021/07/04 10:09:11 by jacher           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS = srcs/main.c \
srcs/print.c \
srcs/philosopher.c \
srcs/init_and_close.c \
srcs/state_tracking.c \
srcs/parsing.c \
srcs/time.c \

OBJS = ${SRCS:.c=.o}

FLAGS = -Wall -Werror -Wextra

CC = clang -pthread

NAME = philo

LIBS = -lpthread

RM = rm -f

.c.o: philo.h 
	$(CC) $(CFLAGS) -c $< -o $(<:.c=.o)

${NAME}: ${OBJS} philo.h 
	${CC} ${FLAGS} -o ${NAME} ${OBJS} ${LIBS}

all: ${NAME}

clean:
	${RM} ${OBJS} 

fclean:	clean
	${RM} ${NAME}

re: fclean all

.PHONY:		all clean fclean re 
