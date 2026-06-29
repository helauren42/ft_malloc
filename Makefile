ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME = libft_malloc_$(HOSTTYPE).so
LIB_NAME = libft_malloc.so

SRCS := src/free.c src/internal/chunks.c src/internal/debug.c src/internal/errors.c src/internal/heaps.c src/internal/libft.c src/internal/print.c src/internal/show_alloc.c src/internal/utils.c src/malloc.c src/realloc.c src/show_alloc_mem.c src/show_alloc_mem_ex.c
SRC_DIR=src/
OBJS = $(SRCS:.c=.o)
RM = /usr/bin/rm -rf

CC = cc
CFLAGS = -Wall -Wextra -Werror -g3 -fPIC

# LIBS

H_FILES = -I ./includes/

define 42testfunc
	cd malloc_docs && cc $1 && mv a.out ../a.out
	./malloc_docs/run_linux.sh /usr/bin/time -v ./a.out
	# LD_PRELOAD=libft_malloc.so LD_LIBRARY_PATH=. ./a.out
endef

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) -shared -o $@
	$(RM) $(LIB_NAME)
	ln -s $(NAME) $(LIB_NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(H_FILES) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

42test:
	$(call 42testfunc,test0.c)

update_source:
	python ./scripts/update_makefile_srcs.py

dev: $(NAME)
	$(MAKE) -C tests dev

tests: $(NAME) clean
	$(MAKE) -C tests

.PHONY: all clean fclean re update_source dev tests
