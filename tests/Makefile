MAKEFLAGS += --no-print-directory

RM=/bin/rm

all:

speed:
	c++ -g3 -I ../includes ./src/tester.cpp ./speed/main.cpp -L.. -lft_malloc -Wl,-rpath,.. && ./a.out && $(RM) a.out

dev:
	c++ -g3 -I ../includes ./src/tester.cpp ./dev/main.cpp -L.. -lft_malloc -Wl,-rpath,.. && ./a.out && $(RM) a.out

gdb:
	c++ -g3 -I ../includes ./src/tester.cpp ./dev/main.cpp -L.. -lft_malloc -Wl,-rpath,.. && gdb ./a.out && $(RM) a.out

.PHONY: all speed dev gdb
