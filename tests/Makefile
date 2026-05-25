MAKEFLAGS += --no-print-directory

dev:
	c++ -g3 -I ../includes ./src/tester.cpp ./dev/main.cpp -L.. -lft_malloc -Wl,-rpath,.. && ./a.out && $(RM) a.out

.PHONY: run dev
