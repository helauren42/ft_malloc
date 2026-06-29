MAKEFLAGS += --no-print-directory
HOSTTYPE = test
TEMP_NAME = libft_malloc_$(HOSTTYPE).so
TEST_NAME = test_malloc.so

RM=/bin/rm -rf

define start
	@echo ""
	@echo "- - - - - Starting $@ tests - - - - -"
	@cd .. && python ./scripts/ft_prefix.py PRE
endef

define end
	@cd .. && python ./scripts/ft_prefix.py POST
	@sleep 1
endef

all: $(TEST_NAME)
	# make tests

$(TEST_NAME):
	cd .. && python ./scripts/ft_prefix.py PRE
	$(MAKE) -C .. clean
	$(MAKE) -C .. HOSTTYPE=$(HOSTTYPE) TEST_MODE=1
	cd .. && mv $(TEMP_NAME) ./tests/$(TEST_NAME) && $(RM) $(TEMP_NAME)
	cd .. && python ./scripts/ft_prefix.py POST

tests: $(TEST_NAME)
	@$(MAKE) valid
	@$(MAKE) error
	@$(MAKE) system
	@$(MAKE) speed

valid:
	$(call start, valid)
	@c++ -g3 -I ../includes ./src/tester.cpp ./valid/main.cpp $(TEST_NAME) -Wl,-rpath,. \
	&& LD_LIBRARY_PATH=. ./a.out && $(RM) a.out
	$(call end)

error:
	$(call start, error)
	@python ./error/main.py
	$(call end)

system:
	$(call start, system)
	@python ./system/system.py
	$(call end)

speed:
	$(call start, error)
	@c++ -g3 -I ../includes ./src/tester.cpp ./speed/main.cpp $(TEST_NAME) -Wl,-rpath,. \
	&& LD_LIBRARY_PATH=. ./a.out && $(RM) a.out
	$(call end)

dev:
	$(call start, error)
	@echo ""
	@echo ""
	@echo ""
	@c++ -g3 -I ../includes ./src/tester.cpp ./dev/main.cpp $(TEST_NAME) -Wl,-rpath,. \
	&& LD_LIBRARY_PATH=. ./a.out && $(RM) a.out
	$(call end)

gdb:
	@c++ -g3 -I ../includes ./src/tester.cpp ./dev/main.cpp $(TEST_NAME) -Wl,-rpath,.. \
		&& LD_LIBRARY_PATH=. gdb -ex "set confirm off" -ex "start" ./a.out && $(RM) a.out

fclean:
	$(RM) $(TEST_NAME)

re: fclean all

.PHONY: all tests valid error system speed dev gdb fclean re
