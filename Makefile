NAME = libft_malloc.so

SRCS := src/free.c src/internal/blocks.c src/internal/debug.c src/internal/errors.c src/internal/libft.c src/internal/print.c src/internal/utils.c src/internal/zones.c src/malloc.c src/show_alloc_mem.c
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

update_source:
	python ./scripts/update_makefile_srcs.py

dev: $(NAME) clean
	$(CC) -g3 -I ./includes ./tests/dev/main.c -L. -lft_malloc -Wl,-rpath,. && ./a.out && $(RM) a.out

# TODO add test later where it actually uses .so
# dev: $(NAME)
# 	$(CC) -g3 -I ./includes ./tests/test0.c -L. -lft_malloc && ./a.out && $(RM) a.out
test: $(NAME) clean
	$(MAKE) -C tests run

.PHONY: all clean fclean re dev tests
