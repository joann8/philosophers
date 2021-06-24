# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jacher <jacher@student.42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/11/11 09:41:42 by jacher            #+#    #+#              #
#    Updated: 2021/06/24 16:16:32 by jacher           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS = srcs/main.c \
srcs/print_help.c \
srcs/philosopher.c \
srcs/parsing.c \

OBJS = ${SRCS:.c=.o}

FLAGS = -Wall -Werror -Wextra

CC = clang -pthread

NAME = philo

LIBS = -lpthread

RM = rm -f

${NAME}: ${OBJS} philo.h 
	${CC} ${FLAGS} -o ${NAME} ${OBJS} ${LIBS}

all: ${NAME}

clean:
	${RM} ${OBJS} 

fclean:	clean
	${RM} ${NAME}

re: fclean all

.PHONY:		all clean fclean re 
