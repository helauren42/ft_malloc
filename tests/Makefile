MAKEFLAGS += --no-print-directory
LIB_PATH = ../ft_libft_malloc.so

RM=/bin/rm

define header
	@echo ""
	@echo "- - - - - Starting $@ tests - - - - -"
endef

all:
	cd .. && make re
	@$(MAKE) valid
	@$(MAKE) error
	@$(MAKE) valtests
	@$(MAKE) speed

valid:
	$(call header, valid)
	@c++ -g3 -I ../includes ./src/tester.cpp ./valid/main.cpp $(LIB_PATH) -Wl,-rpath,.. && ./a.out && $(RM) a.out
	@sleep 1

error:
	$(call header, error)
	@python ./error/main.py
	@sleep 1

valtests:
	$(call header, valtests)
	@python ./system/valtests.py
	@sleep 1

speed:
	cd .. && make re
	$(call header, error)
	@c++ -g3 -I ../includes ./src/tester.cpp ./speed/main.cpp $(LIB_PATH) -Wl,-rpath,.. && ./a.out && $(RM) a.out
	@sleep 1

dev:
	cd .. && make re
	@echo ""
	@echo ""
	@echo ""
	@c++ -g3 -I ../includes ./src/tester.cpp ./dev/main.cpp $(LIB_PATH) -Wl,-rpath,.. && ./a.out && $(RM) a.out

gdb:
	@c++ -g3 -I ../includes ./src/tester.cpp ./dev/main.cpp $(LIB_PATH) -Wl,-rpath,.. && gdb -ex "set confirm off" -ex "start" ./a.out && $(RM) a.out

.PHONY: all valid error speed dev gdb
