NAME = libft_malloc.so

SRCS := $(wildcard src/*.c)
SRC_DIR=src/
OBJS = $(SRCS:.c=.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror -g3 -fPIC

# LIBS

H_FILES = -I ./includes/

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) -shared -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(H_FILES) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

dev: $(NAME)
	$(CC) -g3 -I ./includes ./tests/src/test0.c -L. -lft_malloc -Wl,-rpath,. && ./a.out && $(RM) a.out

# TODO add test later where it actually uses .so
# dev: $(NAME)
# 	$(CC) -g3 -I ./includes ./tests/test0.c -L. -lft_malloc && ./a.out && $(RM) a.out
test: $(NAME)
	$(MAKE) -C tests run

.PHONY: all clean fclean re dev tests
