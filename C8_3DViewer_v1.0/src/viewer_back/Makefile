CC=gcc
# CFLAG= -Wall -Wextra -Werror -std=c11 
CFLAG= -g -std=c11
PROJECT_NAME=3dviewer
TEST?=$(patsubst %.check, % , $(notdir $(wildcard lib_tests/*.check)))
TEST_CHECK=$(patsubst %, lib_tests/%.check, $(TEST))
TEST_C = $(patsubst %.check, test_%.c , $(notdir $(TEST_CHECK)))
TEST_RUN_FILES = $(patsubst %, %.run, $(TEST_C))
CMDSEP = ;
SOURSES= $(wildcard $(PROJECT_NAME)_*.c)
OBJECT= $(patsubst %.c, %.o, $(SOURSES))
VALGRIND?=
GCOV?=
DEBUG?= # DEBUG=-g
LOG?=
LOGFILE?= # LOGFILE=save

ifeq ($(shell uname),Linux) # Linux
	ADD_LIB =`pkg-config --cflags --libs check` -lm
	BREW = .
	SUDO = /usr/bin/sudo -S
	COLOR=-e "\x1b[32m"
	RESET_COLOR="\x1b[0m"

ifeq ($(shell cat /etc/issue | grep -o Alpine), Alpine)
	ADD_LIB= -lcheck -lm -lrt -lpthread
	SUDO =
	COLOR=-e "\x1b[32m"
	RESET_COLOR="\x1b[0m"
endif
endif

ifeq ($(shell uname),Darwin) # MacOS
	OPEN_CMD = open
	ADD_LIB = -lcheck
	BREW := ${HOME}/goinfre/homebrew
	SUDO=
	COLOR = "\033[32m"
	RESET_COLOR = "\033[0m"
endif

all: $(PROJECT_NAME).a

print:
	echo $(TEST1)
	echo $(TEST_CHECK)

build: all

rebuild: clean all

$(PROJECT_NAME).a: create_objects
	$(SUDO) ar rc $(PROJECT_NAME).a $(OBJECT)
	ranlib $(PROJECT_NAME).a

create_objects: $(SOURSES)
	$(foreach el, $(SOURSES), $(SUDO) $(CC) $(CFLAG) $(GCOV) -c $(el)  -o $(el:.c=.o) $(CMDSEP))

test_one: create_objects $(PROJECT_NAME).a test.c 
	-@rm -f test_one
	$(CC) $(CFLAG) test.c -g -o  test_one $(PROJECT_NAME).a -lm
	@rm -f *.o

# check:
# ifeq ("","$(wildcard ./CPPLINT.cfg)") # если файла нет, копируем его.
# 	cp -f ../materials/linters/CPPLINT.cfg ./CPPLINT.cfg
# endif
# 	python ../materials/linters/cpplint.py --extensions=c $^

clang_format:
ifeq ("","$(wildcard ./.clang-format)") # если файла нет, копируем его.
	cp -f ../../materials/linters/.clang-format ./.clang-format
endif
	clang-format -n *.c *.h
	rm -f ./.clang-format

clang_edit:
ifeq ("","$(wildcard ./.clang-format)") # если файла нет, копируем его.
	cp -f ../../materials/linters/.clang-format ./.clang-format
endif
	clang-format -i *.c *.h
	rm -f ./.clang-format

gcov_report:
	make gcov_run GCOV="--coverage"
	
test: $(PROJECT_NAME).a create_tests clean_test clean

test_leaks: 
ifeq  ($(shell uname),Linux)
	make test VALGRIND="CK_FORK=no valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s" LOG=">>leaks.LOG 2>&1"
endif
ifeq ($(shell uname),Darwin) # MacOS
	make test VALGRIND="CK_FORK=no leaks -atExit --" LOG=">>leaks.LOG"
endif

docker_report:
	bash shell_script/docker_report.sh gcov_report $(DEBUG)

docker_test:
	bash shell_script/docker_report.sh test $(DEBUG)

docker_leaks:
	bash shell_script/docker_report.sh test_leaks $(DEBUG)

gcov_run: $(PROJECT_NAME).a create_objects create_tests gcov_report_generate clean_test clean

gcov_report_generate:
	bash shell_script/report.sh

create_tests:
	-@test -f full_leaks.LOG && rm -f full_leaks.LOG
	$(foreach element, $(TEST_CHECK), $(SUDO) sh -c "checkmk $(element) > $(patsubst %.check, test_%.c, $(notdir $(element)))" $(CMDSEP))
	$(foreach element, $(TEST_C),$(SUDO) $(CC) $(GCOV) $(DEBUG) $(element) $(PROJECT_NAME).a -o $(element).run $(ADD_LIB) $(CMDSEP))
	-@$(foreach element, $(TEST_RUN_FILES), \
		$(SUDO) echo ------------------- $(CMDSEP) \
		$(SUDO) echo $(COLOR)$(element:.c.run= )$(RESET_COLOR) $(CMDSEP) \
		$(SUDO) sh -c "$(VALGRIND) ./$(element) $(LOG) "$(CMDSEP) \
		$(SUDO) bash shell_script/leaks.sh $(LOGFILE) $(CMDSEP))
	@echo ------------------- 
	-@test -f full_leaks.LOG && cp full_leaks.LOG lcov_report/full_leaks.LOG ; rm -f full_leaks.LOG

clean_test:
	$(SUDO) rm -rf test_*.c test_*.c.run *.gcno *.gcda *.gcov main_coverage.info test_*.o *.a
	$(SUDO) rm -rf *.dSYM

clean: 
	$(SUDO) rm -rf *.o *.a test_one

.PHONY: all clean check lcov create_tests_c create_tests_run test_run gcov_report test $(PROJECT_NAME).a
