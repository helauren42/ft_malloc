NAME = libft_malloc.so

SRCS := $(wildcard src/*.c)
SRC_DIR=src/
OBJS = $(SRCS:.c=.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror -g3 -fPIC

# LIBS

H_FILES = -I ./includes/

all: $(NAME)
	$(MAKE) -C clean

$(NAME): $(OBJS)
	$(CC) $(OBJS) -shared -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(H_FILES) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

# TODO add test later where it actually uses .so
# dev: $(NAME)
# 	$(CC) -g3 -I ./includes ./tests/test0.c -L. -lft_malloc && ./a.out && $(RM) a.out
dev: $(NAME)
	$(CC) -g3 -I ./includes ./tests/test0.c -L. -lft_malloc -Wl,-rpath,. && ./a.out && $(RM) a.out

.PHONY: all clean fclean re
