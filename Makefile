NAME = libft_malloc.so
TEST_NAME = ft_libft_malloc.so

SRCS := src/free.c src/internal/chunks.c src/internal/debug.c src/internal/errors.c src/internal/heaps.c src/internal/libft.c src/internal/print.c src/internal/show_alloc.c src/internal/utils.c src/malloc.c src/realloc.c src/show_alloc_mem.c src/show_alloc_mem_ex.c
SRC_DIR=src/
OBJS = $(SRCS:.c=.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror -g3 -fPIC

# LIBS

H_FILES = -I ./includes/

all: $(NAME) $(TEST_NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) -shared -o $@

$(TEST_NAME): $(OBJS)
	python ./scripts/ft_prefix.py PRE
	$(CC) $(OBJS) -shared -o $@
	python ./scripts/ft_prefix.py POST

%.o: %.c
	$(CC) $(CFLAGS) $(H_FILES) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

update_source:
	python ./scripts/update_makefile_srcs.py

dev: $(NAME)
	$(MAKE) -C tests dev

tests: $(NAME) clean
	$(MAKE) -C tests

.PHONY: all clean fclean re update_source dev tests
