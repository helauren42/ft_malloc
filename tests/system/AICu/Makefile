NAME = AMAZING

SRCS = readLine.c \
		acIs1.c \
		init.c \
		main.c \
		utils.c \
		turns.c \

OBJS = $(SRCS:.c=.o)

CC = cc

RM = rm -rf

CFLAGS = -Wall -Werror -Wextra -g

all: $(NAME)

$(NAME): $(OBJS)
	$(MAKE) -C libft
	$(MAKE) -C ft_printf
	$(CC) $(OBJS) ./ft_printf/libftprintf.a ./libft/libft.a -o $@

.c.o:
	$(CC) -g $(CFLAGS) -c $< -o $@

clean:
	$(MAKE) -C libft clean
	$(MAKE) -C ft_printf clean
	$(RM) $(OBJS)

fclean: clean
	$(MAKE) -C libft fclean
	$(MAKE) -C ft_printf fclean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
