CC = gcc
CFLAGS =-Wall -Wextra -Werror -std=c11 -lm
CFLAGS_GCOV =-fprofile-arcs -ftest-coverage
CFLAGS_TEST =-Wall -Wextra -Werror -std=c11 -lm -lcheck -lpthread

# OS=$(shell uname)
# ifeq ($(OS), Linux)
#   CFLAGS_TEST=-lcheck -lpthread -lrt -lm -lsubunit
#   OPEN=xdg-open
# else
#   CFLAGS_TEST=-lcheck
#   OPEN=open
# endif

SOURCES = s21_string.c
SOURCES_TEST = s21_string_test.c

BUILD_DIR = .
OUTFILE_TEST = s21_string_test

all: s21_string.a test gcov_report

clean:
	rm -rf ./lcov_report
	rm *.o *.a *.gcno *.gcda *.gcov *.info $(BUILD_DIR)/$(OUTFILE_TEST)

test:
	$(CC) $(CFLAGS_TEST) $(SOURCES) $(SOURCES_TEST) -o $(BUILD_DIR)/$(OUTFILE_TEST)
	$(BUILD_DIR)/$(OUTFILE_TEST)

s21_string.a:
	$(CC) $(CFLAGS) -c $(SOURCES) -o s21_string.o
	ar rc s21_string.a s21_string.o

gcov_report:
	$(CC) $(CFLAGS_TEST) $(CFLAGS_GCOV) $(SOURCES) $(SOURCES_TEST) -o $(BUILD_DIR)/$(OUTFILE_TEST)
	$(BUILD_DIR)/$(OUTFILE_TEST)
	gcov s21_string
	gcov -f s21_string.gcda

lcov_report:
	lcov --capture --directory . --output-file main_coverage.info
	genhtml main_coverage.info --output-directory lcov_report

debug:
	$(CC) $(CFLAGS_TEST) -g $(SOURCES) $(SOURCES_TEST) -o $(BUILD_DIR)/$(OUTFILE_TEST)

lcov:
	brew install lcov

brew:
	#cd ~/goinfre
	#git clone https://github.com/Homebrew/brew homebrew
	#eval "$(~/goinfre/homebrew/bin/brew shellenv)"

valgrind_install:
	brew tap LouisBrunner/valgrind
	brew install --HEAD LouisBrunner/valgrind/valgrind

valgrind:
	# valgrind ./[ваш бинарник] [аргументы] --leak-check=full
	valgrind ./s21_string_test --leak-check=full

style:
	cp ../materials/linters/CPPLINT.cfg .
	python3 ../materials/linters/cpplint.py --extensions=c *.c *.h

check:
	cppcheck --enable=all --suppress=missingIncludeSystem *.c

leaks: test
	CK_FORK=no leaks --atExit -- $(BUILD_DIR)/$(OUTFILE_TEST)
