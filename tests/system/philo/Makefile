# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: helauren <helauren@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/19 17:41:16 by helauren          #+#    #+#              #
#    Updated: 2023/12/01 18:27:24 by helauren         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

CC = gcc

CFLAGS = -Wall -Werror -Wextra

THREAD_FLAG = -pthread

# SANITIZE_FLAG = -fsanitize=thread

# SANITIZE_FLAG = -fsanitize=address

SRCS = $(shell find -iname "*.c" -not -path "./libft/*")

OBJS = $(SRCS:.c=.o)

all:$(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(THREAD_FLAG) $(SANITIZE_FLAG) $(OBJS) -o $(NAME)

.c.o:
	$(CC) -g $(CFLAGS) $(SANITIZE_FLAG) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

phony: all re clean fclean
