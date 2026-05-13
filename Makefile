NAME = libft_malloc.so

SRCS := $(wildcard src/*.c)
SRC_DIR=src/
OBJS = $(SRCS:.c=.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror -g3 -fPIC

# LIBS

H_FILES = -I ./includes/

all: $(NAME)
	@$(RM) $(OBJ)

$(NAME): $(OBJS)
	$(CC) $(OBJS) -shared -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(H_FILES) -c $< -o $@

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

# TODO add test later where it actually uses .so
dev: $(NAME)
	$(CC) -g3 -I ./includes ./tests/test0.c -L. -lft_malloc && ./a.out && $(RM) a.out

.PHONY: all clean fclean re
